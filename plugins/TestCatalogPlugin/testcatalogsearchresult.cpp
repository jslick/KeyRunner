/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "testcatalogsearchresult.h"

#include <QDebug>

TestCatalogSearchResult::TestCatalogSearchResult(const QString& id, const QString& display)
    : id(id), display(display)
{
}

bool TestCatalogSearchResult::isDynamicallyAllocated() const
{
    return false;
}

const QString& TestCatalogSearchResult::getId() const
{
    return this->id;
}

const QString& TestCatalogSearchResult::getDisplay() const
{
    return this->display;
}

const QString& TestCatalogSearchResult::getSearchTerm() const
{
    return this->searchTerm;
}

void TestCatalogSearchResult::setSearchTerm(const QString& searchTerm)
{
    this->searchTerm = searchTerm;
}

bool TestCatalogSearchResult::execute()
{
    qDebug() << "Execute" << this->getDisplay();

    if (this->callback)
        this->callback(*this);

    return true;
}

void TestCatalogSearchResult::setCallback(std::function<void(TestCatalogSearchResult&)> callback)
{
    this->callback = callback;
}
