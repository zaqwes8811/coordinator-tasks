#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T22:54:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = coordinator
TEMPLATE = app

#CONFIG += \
#OnBoardRelease
#TestOnBoardRelease

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
ASL=/home/zaqwes/work/third_party/asl_1.0.43
LOCAL_3RDPARTY=../../3rdparty

OnBoardRelease:DEFINES += G_I_WANT_USE_IT

# View
SOURCES += \
  src/view/mainwindow.cc \
    src/canary/model.cc \
    src/view/view.cc \
    src/fake_store.cc \
    src/canary/values.cc \
    src/canary/entities.cc \


CONFIG(OnBoardRelease) {
SOURCES += \
  src/main.cc
}

# scope
CONFIG(TestOnBoardRelease) {
SOURCES += \
  src/trash_test.cc \
  src/canary/filters_test.cc \
  src/canary/model_test.cc \
  src/canary/pq_queries_test.cc \
  src/canary/entities_test.cc \
  src/htable_adapt_test.cc \
  src/view/app_spec_test.cc \
  launcher.cc \
  $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/gmock-gtest-all.cc
}

# Business logic
SOURCES += \
  src/canary/renders.cc \
  src/canary/filters.cc \
  src/canary/pq_queries.cc

HEADERS  += src/view/mainwindow.h \
    src/canary/isolation.h \
    src/top/error_handling.h \
    src/view/view.h \
    src/top/common.h \
    src/fake_store.h \
    src/canary/values.h

FORMS    += src/view/mainwindow.ui

INCLUDEPATH += src

INCLUDEPATH += \
  $$BOOST \
  $$ASL \
  $$LOCAL_3RDPARTY/loki-0.1.7/include \
  $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/

# -lpthread -lrt
LIBS += -lpq -lpqxx

OTHER_FILES += \
    ReadMe.md \
    forge.txt \
    fail_statistic.txt
