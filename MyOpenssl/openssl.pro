#-------------------------------------------------
#
# Project created by QtCreator 2016-12-22T10:54:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openssl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ModelColumnRO.cpp \
    sslconf.cpp \
    actions.cpp

HEADERS  += mainwindow.h \
    myopenssl.h \
    ModelColumnRO.h \
    sslconf.h \
    actions.h

FORMS    += mainwindow.ui
