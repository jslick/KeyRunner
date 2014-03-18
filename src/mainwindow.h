/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "index.h"
#include <keyrunnerui.h>

#include <QMainWindow>
#include <QList>

class QSystemTrayIcon;
class SearchLineEdit;
class ResultsWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void bringToFront();

protected:
    void paintEvent(QPaintEvent* event);

    void keyPressEvent(QKeyEvent* event);

private slots:
    void fetchResults(QString searchTerm);
    void updateResults(QString searchTerm, QList<SearchResultInterface*> results);
    void executeResult();

    void shrink();
    void shrinkAgain();

private:
    void loadPlugins();

    void drawRoundedRect(
            QPainter& painter, int x, int y, int w, int h,
            int radiusTR, int radiusBR, int radiusBL, int radiusTL,
            bool doFill, const QColor* fillColor,
            bool doLine = false, const QColor* lineColor = 0, int lineWidth = 1,
            bool antiAlias = true
            );

    Ui::MainWindow*     ui;
    SearchLineEdit*     searchLineEdit;
    ResultsWidget*      resultsWidget;
    QSystemTrayIcon*    trayIcon;

    KeyRunnerUi pluginUi;
    Index       index;
    QString     lastSearchTerm;
};

#endif // MAINWINDOW_H
