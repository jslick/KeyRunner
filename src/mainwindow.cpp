#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resultswidget.h"

#include <keyrunnerplugininterface.h>
#include <searchresultinterface.h>

#include <UGlobalHotkey/uglobalhotkeys.h>

#include <QDir>
#include <QPainter>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QPluginLoader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    resultsWidget(new ResultsWidget),
    trayIcon(new QSystemTrayIcon(this)),
    pluginUi(this)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/app/icon.png"));

    QMenu* trayMenu = new QMenu(this);
    trayMenu->addAction(tr("Exit"), this, SLOT(close()));
    this->trayIcon->setContextMenu(trayMenu);
    this->trayIcon->setIcon(this->windowIcon());
    this->trayIcon->show();

    UGlobalHotkeys* hotkeyManager = new UGlobalHotkeys();   // TODO:  cleanup
    try
    {
        hotkeyManager->registerHotkey("Ctrl+Alt+Space");
    }
    catch (UException& e)
    {
        qDebug() << "Unable to register hotkey:  " << e.what();
    }

    connect(hotkeyManager, &UGlobalHotkeys::activated, [this](size_t id)
    {
        if (this->isVisible())
            this->hide();
        else
            this->bringToFront();
    });

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    // center
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    this->move(x(), y() - 160 / 2); // TODO:  extract height

    QLineEdit* lineSearch = this->centralWidget()->findChild<QLineEdit*>("lineSearch");
    if (lineSearch)
    {
        connect(lineSearch, SIGNAL(textChanged(QString)),
                SLOT(fetchResults(QString))
                );
        connect(lineSearch, SIGNAL(returnPressed()),
                SLOT(executeResult())
                );
    }

    connect(&this->index, SIGNAL(resultsFound(QString, QList<SearchResultInterface*>)),
            SLOT(updateResults(QString, QList<SearchResultInterface*>))
            );

    this->resultsWidget->setFixedHeight(160);

    this->loadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bringToFront()
{
    this->show();

    QLineEdit* lineSearch = this->centralWidget()->findChild<QLineEdit*>("lineSearch");
    if (!lineSearch)
        return;

    lineSearch->raise();
    lineSearch->activateWindow();
    lineSearch->selectAll();
    lineSearch->setFocus();
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);
    QColor fillColor(0, 0, 0, 200);
    static const int radius = 8;
    this->drawRoundedRect(
                painter, 0, 0, this->width(), this->height(),
                radius, radius, radius, radius,
                true, &fillColor
                );
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event && event->key() == Qt::Key_Escape)
    {
        QLineEdit* lineSearch = this->centralWidget()->findChild<QLineEdit*>("lineSearch");
        if (lineSearch && lineSearch->text().length())
            lineSearch->setText("");
        else
            this->hide();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::fetchResults(QString searchTerm)
{
    if (searchTerm.length() == 0)
    {
        this->resultsWidget->hide();
        this->centralWidget()->layout()->removeWidget(this->resultsWidget);
        return;
    }

    this->index.search(searchTerm);
}

void MainWindow::updateResults(QString searchTerm, QList<SearchResultInterface*> results)
{
    if (this->resultsWidget->isVisible() == false && results.length())
    {
        this->centralWidget()->layout()->addWidget(this->resultsWidget);
        this->resultsWidget->show();
    }

    if (searchTerm != this->lastSearchTerm)
        this->resultsWidget->clear();

    for (SearchResultInterface* result : results)
    {
        this->resultsWidget->addResult(result);
    }

    this->lastSearchTerm = searchTerm;
}

void MainWindow::executeResult()
{
    if (!this->resultsWidget)
        return;

    SearchResultInterface* result = this->resultsWidget->getCurrentResult();
    if (result)
    {
        bool hideWindow = result->execute();

        QLineEdit* lineSearch = this->centralWidget()->findChild<QLineEdit*>("lineSearch");
        if (lineSearch)
            lineSearch->setText("");

        if (hideWindow)
            this->hide();
    }
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    bool success = pluginsDir.cd("plugins");
    if (!success)
    {
        qDebug() << "Could not open plugins directory";
        return;
    }

    for (QString fileName : pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();
        if (plugin)
        {
            KeyRunnerPluginInterface* pluginInterface = qobject_cast<KeyRunnerPluginInterface*>(plugin);
            if (pluginInterface)
            {
                qDebug() << "Is KeyRunnerPluginInterface";

                pluginInterface->initPlugin(&this->pluginUi);

                QList<CatalogInterface*> catalogs = pluginInterface->getCatalogs();
                for (CatalogInterface* catalog : catalogs)
                {
                    if (catalog)
                        this->index.addCatalog(*catalog);
                }
            }
        }
        else
        {
            qDebug() << "Could not load" << loader.fileName() << "as a plugin: " << loader.errorString();
        }
    }
}

void MainWindow::drawRoundedRect(QPainter& painter, int x, int y, int w, int h, int radiusTR, int radiusBR, int radiusBL, int radiusTL, bool doFill, const QColor* fillColor, bool doLine, const QColor* lineColor, int lineWidth, bool antiAlias)
{
    // http://stackoverflow.com/a/8724493
    int w2 = w / 2;
    int h2 = h / 2;

    if (doLine)
    {
        x += lineWidth / 2;
        y += lineWidth / 2;
        w -= lineWidth;
        h -= lineWidth;
    }

    int T = y;
    int L = x;
    int R = x + w;
    int B = y + h;

    // clamp values to fit within rect
    if (radiusTR > w2)
        radiusTR = w2;
    if (radiusTR > h2)
        radiusTR = h2;

    if (radiusTL > w2)
        radiusTL = w2;
    if (radiusTL > h2)
        radiusTL = h2;

    if (radiusBL > w2)
        radiusBL = w2;
    if (radiusBL > h2)
        radiusBL = h2;

    if (radiusBR > w2)
        radiusBR = w2;
    if (radiusBR > h2)
        radiusBR = h2;

    int diamTR  = radiusTR + radiusTR;
    int diamBR  = radiusBR + radiusBR;
    int diamBL  = radiusBL + radiusBL;
    int diamTL  = radiusTL + radiusTL;

    QPainterPath path;
    if (radiusTR > 0.0)
    {
        path.moveTo(R, T + radiusTR);
        path.arcTo(R-diamTR, T, diamTR, diamTR, 0.0, 90.0);  // TR
    }
    else
        path.moveTo(R,T);

    if (radiusTL > 0.0)
        path.arcTo(L, T, diamTL, diamTL, 90.0, 90.0);  // TL
    else
        path.lineTo(L,T);

    if (radiusBL > 0.0)
        path.arcTo(L, B-diamBL, diamBL, diamBL, 180.0, 90.0);  // BL
    else
        path.lineTo(L,B);

    if (radiusBR > 0.0)
        path.arcTo(R-diamBR, B-diamBR, diamBR, diamBR, 270.0, 90.0);  // BR
    else
        path.lineTo(R,B);

    path.closeSubpath();

    if (antiAlias)
        painter.setRenderHint(QPainter::Antialiasing, true);
    else
        painter.setRenderHint(QPainter::Antialiasing, false);

    if (doFill && fillColor)
        painter.setBrush(*fillColor);
    else if (doFill) // pass doFill and None for fillColor to use current brush
        ;
    else
        painter.setBrush(Qt::NoBrush);

    if (lineWidth != 0 && doLine && lineColor)
    {
        QPen pen(*lineColor, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
    }
    else
        painter.setPen(Qt::NoPen);

    painter.drawPath(path);
}
