#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T22:54:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coordinator
TEMPLATE = app


SOURCES += \
    src/view/trash.cc \
    src/view/mainwindow.cc \
    src/view/main.cc

#SOURCES += ../src/model/models.cc

HEADERS  += src/view/mainwindow.h

FORMS    += src/view/mainwindow.ui

INCLUDEPATH += src
