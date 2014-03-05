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
