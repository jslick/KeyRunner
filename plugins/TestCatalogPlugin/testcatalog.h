#ifndef TESTCATALOG_H
#define TESTCATALOG_H

#include "cataloginterface.h"
#include "testcatalogsearchresult.h"

class UiInterface;

class TestCatalog : public CatalogInterface
{
public:
    TestCatalog(UiInterface& ui);

    void search(const QString& searchTerm, ResultCallback callback, SearchFinishCallback finishCallback);

private:
    UiInterface& ui;

    TestCatalogSearchResult exitSearchResult;
    TestCatalogSearchResult exerciseSearchResult;
};

#endif // TESTCATALOG_H
