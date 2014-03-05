#pragma once

#include <QtPlugin>

class SearchResultInterface
{
public:
    virtual ~SearchResultInterface() {}

    virtual const QString& getId() const = 0;

    virtual const QString& getDisplay() const = 0;

    virtual const QString& getSearchTerm() const = 0;

    virtual void execute() = 0;
};


#define SearchResultInterface_iid "KeyRunner.SearchResultInterface"
Q_DECLARE_INTERFACE(SearchResultInterface, SearchResultInterface_iid)

