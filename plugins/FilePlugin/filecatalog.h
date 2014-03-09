#ifndef FILECATALOG_H
#define FILECATALOG_H

#include "filecatalogdb.h"
#include "fileindexthread.h"

#include <cataloginterface.h>
#include <QReadWriteLock>

class FileSearchResult;

class FileCatalog : public CatalogInterface
{
public:
    FileCatalog();

    void search(const QString &searchTerm, ResultCallback callback, SearchFinishCallback finishCallback);

private:
    void startIndex();

    FileSearchResult* makeResult(const QString& display, const QString& filename);

    FileCatalogDb   db;
    QReadWriteLock  dbLock;
    FileIndexThread dbThread;
};

#endif // FILECATALOG_H
