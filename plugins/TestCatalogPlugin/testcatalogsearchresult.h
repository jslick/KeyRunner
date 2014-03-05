#ifndef TESTCATALOGSEARCHRESULT_H
#define TESTCATALOGSEARCHRESULT_H

#include <searchresultinterface.h>

#include <functional>

class TestCatalogSearchResult : public SearchResultInterface
{
public:
    TestCatalogSearchResult(const QString& id, const QString& display);

    virtual const QString& getId() const;

    virtual const QString& getDisplay() const;

    virtual const QString& getSearchTerm() const;

    void setSearchTerm(const QString& searchTerm);

    virtual void execute();

    void setCallback(std::function<void(TestCatalogSearchResult&)> callback);

private:
    QString id;
    QString display;
    QString searchTerm;

    std::function<void(TestCatalogSearchResult&)> callback;
};

#endif // TESTCATALOGSEARCHRESULT_H
