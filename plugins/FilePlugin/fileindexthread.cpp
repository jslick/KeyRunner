/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "fileindexthread.h"
#include "filecatalogdb.h"

#include <finalizer.h>
#include <QStringList>
#include <QStack>
#include <QDir>
#ifdef Q_OS_WIN
#  include <QSettings>
#  include <userenv.h>
#  include <Shlobj.h>
#  include <cpl.h>
#endif
#include <QReadWriteLock>
#include <QLibrary>
#include <QDebug>

static QString expandEnvironmentPath(const QString& path)
{
#ifdef Q_OS_WIN
    WCHAR dst[MAX_PATH];
    DWORD nCopied = ExpandEnvironmentStringsW(path.toStdWString().c_str(), dst, sizeof(dst) / sizeof(WCHAR));
    if (nCopied == 0)   // failure
        return path;
    const QString expanded = QString::fromWCharArray(dst);
    return expanded;
#else
    // TODO (wordexp)
    return path;
#endif
}

#ifdef Q_OS_WIN
static QString getWindowsSystemPath()
{
    WCHAR pSystemPath[MAX_PATH];
    UINT nCopied = GetSystemDirectoryW(pSystemPath, sizeof(pSystemPath) / sizeof(WCHAR));
    if (nCopied)
        return QString::fromWCharArray(pSystemPath, nCopied);
    else
        return "";
}
#endif

#ifdef Q_OS_WIN
static bool readCpl(const QString& cplFilename, FileCatalogDb& db, QReadWriteLock& dbLock)
{
    QLibrary libCpl(cplFilename);
    bool success = libCpl.load();
    if (!success)
    {
        qDebug() << "Unable to load cpl" << cplFilename;
        return false;
    }
    Finalizer unloadLib([&libCpl]() { libCpl.unload(); });
    Q_UNUSED(unloadLib);

    APPLET_PROC proc = (APPLET_PROC) libCpl.resolve("CPlApplet");
    if (!proc)
    {
        qDebug() << "Cannot resolve symbol from" << cplFilename << ": " << libCpl.errorString();
        return false;
    }

    LONG rv = proc(0, CPL_INIT, 0, 0);
    if (rv == 0)
        return false;

    Finalizer callCplExit([&proc] { if (proc) proc(0, CPL_EXIT, 0, 0); });
    Q_UNUSED(callCplExit);

    HMODULE hLibInst = GetModuleHandle(libCpl.fileName().toStdWString().c_str());

    const QString systemPath = getWindowsSystemPath();
    const QDir systemDir(systemPath);
    const QString controlExePath = systemDir.absoluteFilePath("control.exe");

    LONG nDialogs = proc(0, CPL_GETCOUNT, 0, 0);
    if (nDialogs)
        dbLock.lockForWrite();
    for (LONG i = 0; i < nDialogs; i++)
    {
        CPLINFO cplInfo;
        memset(&cplInfo, 0, sizeof(cplInfo));
        proc(0, CPL_INQUIRE, i, reinterpret_cast<LPARAM>(&cplInfo));
        // MSDN documentation says CPL_INQUIRE returns zero upon success.  But, for some reason,
        // it returns non-zero for some cpl (e.g. main.cpl).  We will ignore the return value.

        WCHAR cplNameBuffer[256];
        const int nCopied = LoadStringW(hLibInst, cplInfo.idName, cplNameBuffer, sizeof(cplNameBuffer) / sizeof(WCHAR));
        if (nCopied == 0)
        {
            qDebug() << "Unable to load string for" << libCpl.fileName() << i;
            continue;
        }

        const QString cplName = QString::fromWCharArray(cplNameBuffer, nCopied);
        db.add(cplName, cplName,
               controlExePath,
                { libCpl.fileName() + ",@" + QString::number(i) }
               );
    }
    if (nDialogs)
        dbLock.unlock();

    callCplExit.doNow();

    return true;
}
#endif

FileIndexThread::FileIndexThread(FileCatalogDb& db, QReadWriteLock& dbLock, QObject* parent) :
    QThread(parent),
    db(db), dbLock(dbLock)
{
}

void FileIndexThread::run()
{
    struct PathToIndex
    {
        QString path;
        bool    recursive;
        QStringList filter;
    };

#ifdef Q_OS_UNIX
    static const QList<PathToIndex> pathsToIndex = {
        { "/usr/share/applications", true, { "*.desktop" } },
        { QDir(QDir::homePath()).relativeFilePath(".local/share/applications"), true, { "*.desktop" } },
        { QDir(QDir::homePath()).relativeFilePath("Desktop"), true, { "*.desktop" } },
    };
#elif defined(Q_OS_WIN)
    QList<PathToIndex> pathsToIndex = {
        { QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders", QSettings::NativeFormat).value("Common Desktop").toString(), true, { "*.lnk" } },
        { QSettings("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders", QSettings::NativeFormat).value("Common Start Menu").toString(), true, { "*.lnk" } },
    };

    {
        // There are newer API functions for this; but they are only for Vista and newer
        WCHAR szPath[MAX_PATH];
        HRESULT hr = SHGetFolderPathW(0, CSIDL_STARTMENU, 0, 0, szPath);
        if (SUCCEEDED(hr))
            pathsToIndex.push_back({ QString::fromWCharArray(szPath), true, { "*.lnk" } });

        hr = SHGetFolderPathW(0, CSIDL_DESKTOPDIRECTORY, 0, 0, szPath);
        if (SUCCEEDED(hr))
            pathsToIndex.push_back({ QString::fromWCharArray(szPath), true, { "*.lnk" } });
    }
#else
#error "Not implemented for this operating system"
#endif

    for (const PathToIndex& pathInfo : pathsToIndex)
    {
        const QString pathString = expandEnvironmentPath(pathInfo.path);
        QStack<QString> dirstack;
        dirstack.push_back(expandEnvironmentPath(pathString));

        while (dirstack.empty() == false)
        {
            QDir path(dirstack.pop());
            path.setNameFilters(pathInfo.filter);

            for (const QString& filename : path.entryList())
            {
                int dot = filename.lastIndexOf('.');
                const QString basename = dot > 0 ? filename.left(dot) : filename;

                dbLock.lockForWrite();
                this->db.add(basename, basename, path.absoluteFilePath(filename), QStringList());
                dbLock.unlock();
            }

            // For now, no symlink traversal to avoid loops
            for (const QString& dirname : path.entryList(QStringList(), QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
            {
                dirstack.push_back(path.absoluteFilePath(dirname));
            }
        }
    }

#ifdef Q_OS_WIN
    const QString systemPath = getWindowsSystemPath();

    for (const QString& filename : QDir(systemPath).entryList({ "*.cpl" }))
    {
        readCpl(filename, this->db, this->dbLock);
    }
#endif

    emit indexDone();
}
