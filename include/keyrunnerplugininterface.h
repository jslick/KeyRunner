/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#pragma once

#include <QtPlugin>
#include <QList>

class UiInterface;
class CatalogInterface;

class KeyRunnerPluginInterface
{
public:
    virtual ~KeyRunnerPluginInterface() {}

    virtual void initPlugin(UiInterface* ui) = 0;

    virtual QList<CatalogInterface*> getCatalogs() = 0;
};

#define KeyRunnerPluginInterface_iid "KeyRunner.KeyRunnerPluginInterface"
Q_DECLARE_INTERFACE(KeyRunnerPluginInterface, KeyRunnerPluginInterface_iid)
