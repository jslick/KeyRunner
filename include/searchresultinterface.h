/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#pragma once

#include <QtPlugin>

class SearchResultInterface
{
public:
    virtual ~SearchResultInterface() {}

    virtual bool isDynamicallyAllocated() const = 0;

    virtual const QString& getId() const = 0;

    virtual const QString& getDisplay() const = 0;

    virtual const QString& getSearchTerm() const = 0;

    virtual bool execute() = 0;
};


#define SearchResultInterface_iid "KeyRunner.SearchResultInterface"
Q_DECLARE_INTERFACE(SearchResultInterface, SearchResultInterface_iid)

