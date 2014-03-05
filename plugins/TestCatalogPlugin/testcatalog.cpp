#include "testcatalog.h"
#include "testcatalogsearchresult.h"

#include <uiinterface.h>

#include <QStringList>
#include <QRegularExpression>
#include <QMainWindow>

#include <functional>

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
    QStringList words = searchTerm.toLower().split(QRegularExpression("\\s+"));
    for (QString word : words)
    {
        if (QString(QObject::tr("exit")).contains(word))
        {
            this->exitSearchResult.setSearchTerm(searchTerm);
            if (callback)
            {
                QList<SearchResultInterface*> results;
                results.push_back(&this->exitSearchResult);
                callback(searchTerm, results);
            }
        }
        if (QString(QObject::tr("exercise")).contains(word))
        {
            this->exerciseSearchResult.setSearchTerm(searchTerm);
            if (callback)
            {
                QList<SearchResultInterface*> results;
                results.push_back(&this->exerciseSearchResult);
                callback(searchTerm, results);
            }
        }
    }

    if (finishCallback)
        finishCallback(searchTerm);
}
