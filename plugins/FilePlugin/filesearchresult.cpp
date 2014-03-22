/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "filesearchresult.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

#ifdef Q_OS_WIN
#  include <windows.h>
#endif

enum De
{
    DeKde,

    DeUnknown
};

static De getDe()
{
#ifdef Q_OS_UNIX
    if (qgetenv("KDE_FULL_SESSION").length())
        return DeKde;
    else
        return DeUnknown;
#else
    return DeUnknown;
#endif
}

static QString getXdgValue(const QString& contents, const QString& key, const QString& accessor = "")
{
    QString fullkey = accessor.length() ? key + "[" + accessor + "]" : key;
    QRegularExpression re("^" + fullkey + "=(.*)$", QRegularExpression::MultilineOption);
    auto match = re.match(contents);
    return match.hasMatch() ? match.captured(1) : "";
}

static QString getXdgExec(const QString& filename)
{
    QFile file(filename);
    bool success = file.open(QFile::ReadOnly | QFile::Text);
    if (!success)
        return "";

    QTextStream in(&file);
    const QString contents = in.readAll();
    file.close();

    // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s06.html

    QString execname = getXdgValue(contents, "Exec");
    if (execname.length())
    {
        if (execname.indexOf("%c"))
        {
            const QString caption = getXdgValue(contents, "Name");
            execname.replace("%c", caption.length() ? caption : "KeyRunner");
        }
        execname = execname.replace(QRegularExpression("%[iuUfFk]"), "");
        return std::move(execname);
    }
    else
        return "";
}

FileSearchResult::FileSearchResult(const QString& id, const QString& display, const QString& filename, const QStringList& args)
    : id(id), display(display), filename(filename), args(args)
{
}

bool FileSearchResult::isDynamicallyAllocated() const
{
    return true;
}

const QString& FileSearchResult::getId() const
{
    return this->id;
}

const QString& FileSearchResult::getDisplay() const
{
    return this->display;
}

const QString& FileSearchResult::getSearchTerm() const
{
    return this->searchTerm;
}

void FileSearchResult::setSearchTerm(const QString& searchTerm)
{
    this->searchTerm = searchTerm;
}

bool FileSearchResult::execute()
{
#ifdef Q_OS_WIN

    const QString exe = this->filename;
    const QString params = this->args.join(' ');
    qDebug() << "Executing" << (exe + params);
    int rv = (int) ::ShellExecuteW(
                0,
                L"open", exe.toStdWString().c_str(), params.toStdWString().c_str(),
                QDir::homePath().toStdWString().c_str(),
                SW_SHOW
                );
    if (rv <= 32)
        qDebug() << "Unable to execute; rv is" << rv;

#else

    if (this->filename.endsWith(".desktop"))
    {
        QString execname = getXdgExec(this->filename);
        if (execname.length())
        {
            if (getDe() == DeKde)
                execname = "kstart --activate " + execname;

            qDebug() << "Executing: " << execname;

            // Change current working directory; startDetached doesn't seem to
            // have an overload that accepts the `program` string having
            // arguments AND a parameter for the process pwd.
            const QDir savedPwd = QDir::current();
            QDir::setCurrent(QDir::homePath());
            QProcess::startDetached(execname);
            QDir::setCurrent(savedPwd.absolutePath());
        }
        else
        {
            QMessageBox::critical(0, QObject::tr("Unable to Launch"),
                                  QObject::tr("Unable to find command to launch.")
                                  );
            return false;
        }
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Unable to Launch"),
                              QObject::tr("I don't know how to launch this file.")
                              );
        return false;
    }

#endif

    return true;
}
