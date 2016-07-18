#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T00:15:30
#
#-------------------------------------------------

QT       += core gui webkitwidgets webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Testing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    websocketserver.cpp \
    webpagebuilder.cpp \
    viewerwindow.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    form.h \
    websocketserver.h \
    webpagebuilder.h \
    viewerwindow.h \
    config.h

FORMS    += mainwindow.ui \
    form.ui \
    viewerwindow.ui

DISTFILES += \
    index.html

CONFIG += c++11
