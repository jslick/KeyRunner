#include "filecatalog.h"
#include "filesearchresult.h"

//#include <QDir>

FileCatalog::FileCatalog()
    : dbThread(db, dbLock)
{
#if 0
    // TODO:  check return values for failure
#ifdef Q_OS_UNIX
    QDir dbdir = QDir(QDir::homePath());
    if (dbdir.exists(".cache/keyrunner") == false)
        dbdir.mkpath(".cache/keyrunner");
    dbdir.cd(".cache/keyrunner");
#elif defined(Q_OS_WIN)
    QDir dbdir = QDir(qgetenv("localappdata"));
    if (dbdir.exists("keyrunner") == false)
        dbdir.mkpath("keyrunner");
    dbdir.cd("keyrunner");
#else
#error "Not implemented for this operating system"
#endif
    const QString dbpath = dbdir.relativeFilePath("files.db");
    if (QFile::exists(dbpath) == false)
#endif // 0
        this->startIndex();
}

void FileCatalog::search(const QString& searchTerm, ResultCallback callback, SearchFinishCallback finishCallback)
{
    if (this->dbThread.isRunning())
        this->dbLock.lockForRead();

    // Captures are by reference - this is currently synchronous.
    // Capture by value if this becomes async.
    this->db.find(searchTerm, [this,&searchTerm,&callback](const QString& display,const QString& filename)
    {
        QList<SearchResultInterface*> results;
        results.push_back(makeResult(display, filename));
        callback(searchTerm, results);
    });

    if (this->dbThread.isRunning())
        this->dbLock.unlock();

    if (finishCallback)
        finishCallback(searchTerm);
}

void FileCatalog::startIndex()
{
    this->dbThread.start();
}

FileSearchResult* FileCatalog::makeResult(const QString& display, const QString& filename)
{
    FileSearchResult* result = new FileSearchResult(filename, display, filename);
    return result;
}
