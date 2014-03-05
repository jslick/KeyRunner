#pragma once

#include <keyrunnerplugininterface.h>

#include <QGenericPlugin>

class UiInterface;
class CatalogInterface;

class TestCatalogPlugin : public QGenericPlugin, public KeyRunnerPluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "TestCatalogPlugin.json")
    Q_PLUGIN_METADATA(IID KeyRunnerPluginInterface_iid FILE "TestCatalogPlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(KeyRunnerPluginInterface)

public:
    TestCatalogPlugin(QObject* parent = 0);

    virtual QObject* create(const QString& key, const QString& specification);

    virtual void initPlugin(UiInterface* ui);

    virtual QList<CatalogInterface*> getCatalogs();

private:
    UiInterface* ui;
};
