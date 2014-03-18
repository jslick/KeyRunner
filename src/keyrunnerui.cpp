/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "keyrunnerui.h"

KeyRunnerUi::KeyRunnerUi(QMainWindow* mainWindow)
    : mainWindow(mainWindow)
{
}

QMainWindow* KeyRunnerUi::getMainWindow()
{
    return this->mainWindow;
}
