#include "keyrunnerui.h"

KeyRunnerUi::KeyRunnerUi(QMainWindow* mainWindow)
    : mainWindow(mainWindow)
{
}

QMainWindow* KeyRunnerUi::getMainWindow()
{
    return this->mainWindow;
}
