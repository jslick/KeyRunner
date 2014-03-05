#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T11:51:40
#
#-------------------------------------------------

QT      += core gui
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testcatalogplugin
TEMPLATE = lib
CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/generic
DESTDIR = ../

INCLUDEPATH += ../../include/

SOURCES += \
    testcatalogplugin.cpp \
    testcatalog.cpp \
    testcatalogsearchresult.cpp

HEADERS += \
    testcatalogplugin.h \
    testcatalog.h \
    testcatalogsearchresult.h
OTHER_FILES += TestCatalogPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
