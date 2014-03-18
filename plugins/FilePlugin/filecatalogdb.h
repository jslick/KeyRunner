/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef FILECATALOGDB_H
#define FILECATALOGDB_H

#include <functional>
#include <QString>
#include <QList>
#include <QHash>

class FileCatalogDb
{
public:
    FileCatalogDb();

    void add(const QString& searchTerm, const QString& display, const QString& filename);

    void find(const QString& searchTerm, std::function<void(const QString& display,const QString& filename)> matchCallback);

private:
    struct Match
    {
        QString display;
        QString filename;
    };

    struct HashValue
    {
        QList<Match> files;
        QHash<QChar,HashValue> subindex;
    };

    const HashValue* matchWord(const QString& word) const;

    QHash<QChar,HashValue> index;
};

#endif // FILECATALOGDB_H
