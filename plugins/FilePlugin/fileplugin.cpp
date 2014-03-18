/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "fileplugin.h"
#include "filecatalog.h"

FilePlugin::FilePlugin(QObject* parent)
    : QObject(parent)
{
}

void FilePlugin::initPlugin(UiInterface* ui)
{
    Q_UNUSED(ui);
}

QList<CatalogInterface*> FilePlugin::getCatalogs()
{
    QList<CatalogInterface*> catalogs;
    catalogs.push_back(new FileCatalog);    // TODO:  cleanup
    return std::move(catalogs);
}
