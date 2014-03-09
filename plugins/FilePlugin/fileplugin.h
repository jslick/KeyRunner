#pragma once

#include <keyrunnerplugininterface.h>
#include <QObject>

class FilePlugin : public QObject, public KeyRunnerPluginInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID KeyRunnerPluginInterface_iid FILE "FilePlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(KeyRunnerPluginInterface)

public:
    FilePlugin(QObject* parent = 0);

    virtual void initPlugin(UiInterface* ui);

    virtual QList<CatalogInterface*> getCatalogs();
};
