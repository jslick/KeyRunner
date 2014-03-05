#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T21:56:20
#
#-------------------------------------------------

QT      += core gui
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keyrunner
TEMPLATE = app

DESTDIR = ../

INCLUDEPATH += $$PWD/../contrib/include/
unix {
    LIBS += $$PWD/../contrib/lib/libUGlobalHotkey.a
    LIBS += -lxcb -lxcb-keysyms
}

INCLUDEPATH += $$PWD/../include/

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    resultswidget.cpp \
    index.cpp \
    keyrunnerui.cpp

HEADERS  += mainwindow.h \
    index.h \
    ../include/uiinterface.h \
    ../include/cataloginterface.h \
    ../include/searchresultinterface.h \
    ../include/keyrunnerplugininterface.h \
    resultswidget.h \
    keyrunnerui.h

FORMS    += mainwindow.ui

RESOURCES += \
    keyrunner.qrc

SUBDIRS = ../plugins/TestCatalogPlugin/
