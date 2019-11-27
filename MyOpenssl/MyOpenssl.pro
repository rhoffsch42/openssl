#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T12:35:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyOpenssl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ModelColumnRO.cpp \
    sslconf.cpp \
    actions.cpp

HEADERS  += mainwindow.h\
    myopenssl.h \
    ModelColumnRO.h \
    sslconf.h \
    actions.h

FORMS    += mainwindow.ui
