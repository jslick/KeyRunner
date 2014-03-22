/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

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

    FileSearchResult* makeResult(const QString& display, const QString& filename, const QStringList& params);

    FileCatalogDb   db;
    QReadWriteLock  dbLock;
    FileIndexThread dbThread;
};

#endif // FILECATALOG_H
