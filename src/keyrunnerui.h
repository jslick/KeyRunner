#ifndef KEYRUNNERUI_H
#define KEYRUNNERUI_H

#include <uiinterface.h>

class QMainWindow;

class KeyRunnerUi : public UiInterface
{
public:
    explicit KeyRunnerUi(QMainWindow* mainWindow);

    virtual QMainWindow* getMainWindow();

private:
    QMainWindow* mainWindow;
};

#endif // KEYRUNNERUI_H
