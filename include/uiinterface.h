/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#pragma once

#include <QObject>

class QMainWindow;

class UiInterface
{
public:
    virtual ~UiInterface() {}

    virtual QMainWindow* getMainWindow() = 0;
};

#define UiInterface_iid "KeyRunner.UiInterface"
Q_DECLARE_INTERFACE(UiInterface, UiInterface_iid)
