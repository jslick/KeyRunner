/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef INDEX_H
#define INDEX_H

#include <QObject>
#include <QList>

class CatalogInterface;
class SearchResultInterface;

class Index : public QObject
{
    Q_OBJECT
public:
    explicit Index(QObject* parent = 0);

    void addCatalog(CatalogInterface& catalog);

    void search(const QString& searchTerm);

signals:
    void resultsFound(QString searchTerm, QList<SearchResultInterface*>);

public slots:

private slots:
    void sendResults(QString searchTerm, QList<SearchResultInterface*> results);

private:
    QList<CatalogInterface*> catalogs;
};

#endif // INDEX_H
