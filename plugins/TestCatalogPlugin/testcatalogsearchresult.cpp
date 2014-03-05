#include "testcatalogsearchresult.h"

#include <QDebug>   // TODO:  REMOVE

TestCatalogSearchResult::TestCatalogSearchResult(const QString& id, const QString& display)
    : id(id), display(display)
{
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

void TestCatalogSearchResult::execute()
{
    qDebug() << "Execute" << this->getDisplay();

    if (this->callback)
        this->callback(*this);
}

void TestCatalogSearchResult::setCallback(std::function<void(TestCatalogSearchResult&)> callback)
{
    this->callback = callback;
}
