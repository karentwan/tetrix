#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T11:23:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tetris
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    configuration.cpp \
    backpanel.cpp \
    shape.cpp \
    barrier.cpp \
    thread.cpp

HEADERS  += widget.h \
    configuration.h \
    backpanel.h \
    shape.h \
    barrier.h \
    thread.h

FORMS    += widget.ui
