#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T01:41:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    wrapper.cpp

HEADERS  += mainwindow.h \
    inc/client.h \
    inc/wrapper.h \
    inc/msg.h \
    inc/a3defs.h

FORMS    += mainwindow.ui
