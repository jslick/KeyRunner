#include "fileindexthread.h"
#include "filecatalogdb.h"

#include <QStringList>
#include <QStack>
#include <QDir>
#ifdef Q_OS_WIN
#  include <QSettings>
#  include <userenv.h>
#  include <Shlobj.h>
#endif
#include <QReadWriteLock>

static QString expandEnvironmentPath(const QString& path)
{
#ifdef Q_OS_WIN
    WCHAR dst[512];
    DWORD nCopied = ExpandEnvironmentStringsW(path.toStdWString().c_str(), dst, sizeof(dst));
    if (nCopied == 0)   // failure
        return path;
    const QString expanded = QString::fromWCharArray(dst);
    return expanded;
#else
    // TODO (wordexp)
    return path;
#endif
}

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
                this->db.add(basename, basename, path.absoluteFilePath(filename));
                dbLock.unlock();
            }

            // For now, no symlink traversal to avoid loops
            for (const QString& dirname : path.entryList(QStringList(), QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
            {
                dirstack.push_back(path.absoluteFilePath(dirname));
            }
        }
    }

    emit indexDone();
}
