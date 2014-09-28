#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T22:54:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coordinator
TEMPLATE = app

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
ASL=/home/zaqwes/work/third_party/asl_1.0.43
LOCAL_3RDPARTY=../3rdparty

# View
SOURCES += \
  src/view/trash.cc \
  src/view/mainwindow.cc \
  src/view/main.cc

# Business logic
SOURCES += \
  src/canary/entities.cc \
  src/_trash/_trash.cc \
  src/canary/app_core.cc \
  src/canary/renders.cc \
  src/canary/filters.cc \
  src/canary/pq_queries.cc

HEADERS  += src/view/mainwindow.h

FORMS    += src/view/mainwindow.ui

INCLUDEPATH += src

INCLUDEPATH += \
  $$BOOST \
  $$ASL \
  $$LOCAL_3RDPARTY/loki-0.1.7/include

# -lpthread -lrt
LIBS += -lpq -lpqxx
