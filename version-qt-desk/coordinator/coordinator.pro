#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T22:54:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coordinator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/model/models.cc

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../../src
