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

INCLUDEPATH += $$PWD/../contrib/
unix {
    LIBS += -L$$OUT_PWD/../contrib/UGlobalHotkey/ -lUGlobalHotkey
    LIBS += -lxcb -lxcb-keysyms
}
win32 {
    LIBS += $$OUT_PWD/../UGlobalHotkey.dll
}

INCLUDEPATH += $$PWD/../include/

SOURCES += \
    main.cpp \
    index.cpp \
    mainwindow.cpp \
    resultswidget.cpp \
    keyrunnerui.cpp

HEADERS  += \
    ../include/uiinterface.h \
    ../include/cataloginterface.h \
    ../include/searchresultinterface.h \
    ../include/keyrunnerplugininterface.h \
    index.h \
    mainwindow.h \
    resultswidget.h \
    keyrunnerui.h

FORMS    += mainwindow.ui

RESOURCES += \
    keyrunner.qrc

SUBDIRS = ../plugins/TestCatalogPlugin/
