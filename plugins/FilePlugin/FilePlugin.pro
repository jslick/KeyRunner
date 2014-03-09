QT      += core
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileplugin
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../

INCLUDEPATH += ../../include/

SOURCES += \
    fileplugin.cpp \
    filecatalog.cpp \
    filesearchresult.cpp \
    filecatalogdb.cpp \
    fileindexthread.cpp

HEADERS += \
    fileplugin.h \
    filecatalog.h \
    filesearchresult.h \
    filecatalogdb.h \
    fileindexthread.h

OTHER_FILES += \
    FilePlugin.json
