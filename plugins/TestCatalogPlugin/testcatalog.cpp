/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "testcatalog.h"
#include "testcatalogsearchresult.h"

#include <uiinterface.h>

#include <functional>
#include <QMainWindow>

TestCatalog::TestCatalog(UiInterface& ui)
    : ui(ui),
      exitSearchResult("exit", QObject::tr("Exit")),
      exerciseSearchResult("exercise", QObject::tr("Exercise"))
{
    this->exitSearchResult.setCallback([this](TestCatalogSearchResult&)
    {
        QMainWindow* mainWindow = this->ui.getMainWindow();
        if (mainWindow)
            mainWindow->close();
    });
}

void TestCatalog::search(const QString& searchTerm, ResultCallback callback, SearchFinishCallback finishCallback)
{
    const QString mySearchTerm = searchTerm.toLower().trimmed();
    if (QString(QObject::tr("exit")).contains(mySearchTerm))
    {
        this->exitSearchResult.setSearchTerm(searchTerm);
        if (callback)
        {
            QList<SearchResultInterface*> results;
            results.push_back(&this->exitSearchResult);
            callback(searchTerm, results);
        }
    }
    if (QString(QObject::tr("exercise")).contains(mySearchTerm))
    {
        this->exerciseSearchResult.setSearchTerm(searchTerm);
        if (callback)
        {
            QList<SearchResultInterface*> results;
            results.push_back(&this->exerciseSearchResult);
            callback(searchTerm, results);
        }
    }

    if (finishCallback)
        finishCallback(searchTerm);
}
