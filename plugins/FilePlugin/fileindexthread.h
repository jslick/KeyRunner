/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef FILEINDEXTHREAD_H
#define FILEINDEXTHREAD_H

#include <QThread>

class FileCatalogDb;
class QReadWriteLock;

class FileIndexThread : public QThread
{
    Q_OBJECT
public:
    explicit FileIndexThread(FileCatalogDb& db, QReadWriteLock& dbLock, QObject* parent = 0);

signals:
    void indexDone();

public slots:

protected:
    void run();

private:
    FileCatalogDb&  db;
    QReadWriteLock& dbLock;
};

#endif // FILEINDEXTHREAD_H
