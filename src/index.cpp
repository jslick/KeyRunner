/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "index.h"

#include <cataloginterface.h>

Index::Index(QObject* parent) :
    QObject(parent)
{
}

void Index::addCatalog(CatalogInterface& catalog)
{
    this->catalogs.push_back(&catalog);
}

void Index::search(const QString& searchTerm)
{
    for (CatalogInterface* catalog : this->catalogs)
    {
        Q_ASSERT(catalog);
        bool foundResults = false;
        catalog->search(searchTerm, [this,&foundResults](const QString& searchTerm,const QList<SearchResultInterface*>& results)
        {
            foundResults = true;
            this->sendResults(searchTerm, results);
        },
        [this,&foundResults](const QString& searchTerm)
        {
            if (!foundResults)
                this->sendResults(searchTerm, std::move(QList<SearchResultInterface*>()));
        });
    }
}

void Index::sendResults(QString searchTerm, QList<SearchResultInterface*> results)
{
    emit resultsFound(searchTerm, results);
}
