#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T00:15:30
#
#-------------------------------------------------

#QT       += core gui webkitwidgets webkit websockets
QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Testing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        websocketserver.cpp \
        config.cpp \
        testview.cpp

HEADERS  += mainwindow.h \
         websocketserver.h \
         config.h \
         testview.h

FORMS    += mainwindow.ui

DISTFILES += \
    vinebooru-sender.user.js

CONFIG += c++11
