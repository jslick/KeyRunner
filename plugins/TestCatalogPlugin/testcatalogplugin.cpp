#include "testcatalogplugin.h"
#include "testcatalog.h"

#include <QDebug>

TestCatalogPlugin::TestCatalogPlugin(QObject* parent) :
    QGenericPlugin(parent),
    ui(0)
{
}

QObject* TestCatalogPlugin::create(const QString& key, const QString& specification)
{
    Q_UNUSED(key);
    Q_UNUSED(specification);

    return 0;
}

void TestCatalogPlugin::initPlugin(UiInterface* ui)
{
    this->ui = ui;
}

QList<CatalogInterface*> TestCatalogPlugin::getCatalogs()
{
    if (!this->ui)
    {
        qDebug() << "Cannot getCatalogs - ui is not given";
        return std::move(QList<CatalogInterface*>());
    }

    QList<CatalogInterface*> catalogs;
    catalogs.push_back(new TestCatalog(*this->ui));    // TODO:  cleanup
    return std::move(catalogs);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(TestCatalogPlugin, GenericPlugin)
#endif // QT_VERSION < 0x050000
