#pragma once

#include <QObject>
#include <QList>
#include <functional>

class SearchResultInterface;

typedef std::function<void(const QString& searchTerm,const QList<SearchResultInterface*>& results)> ResultCallback;
typedef std::function<void(const QString& searchTerm)> SearchFinishCallback;

class CatalogInterface
{
public:
    virtual ~CatalogInterface() {}

    virtual void search(const QString& searchTerm, ResultCallback callback, SearchFinishCallback finishCallback) = 0;
};

#define CatalogInterface_iid "KeyRunner.CatalogInterface"
Q_DECLARE_INTERFACE(CatalogInterface, CatalogInterface_iid)
