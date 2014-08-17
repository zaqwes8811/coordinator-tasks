# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = a.app

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
_3RDPARTY=../3rdparty

# -std=c++11 -pg
QMAKE_CXXFLAGS += -O2  -Wall -Wno-deprecated 

SOURCES += app.cc \ 
    dal.cc
#\
#  ../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
#  ../launcher.cc

INCLUDEPATH += $$BOOST $$_3RDPARTY/loki-0.1.7/include ../
#../../third_party/gmock-1.6.0/fused-src/

LIBS += -lpthread -lrt -ltbb -lpq -lpqxx

OBJECTS_DIR = obj

HEADERS += \
    dal.h \
    domain.h

