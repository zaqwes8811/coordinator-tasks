TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
TARGET = a.app

SOURCES += \
    ../../extern/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
    ./src/tests-launcher.cc \
    ./src/expose_test.cc

INCLUDEPATH += \
	"../../extern/gmock-1.6.0/fused-src" \
  	"/opt/big-3rdparty/boost_1_57_0"

INCLUDEPATH += /usr/include/python2.7

LIBPATH += /opt/big-3rdparty/boost_1_57_0/stage_gcc_x64/lib
LIBS += -lboost_python -lboost_system
LIBS += -lpython2.7 -lpthread -lrt

HEADERS += \
    ./src/foo.h \
    ./src/foo_py.h

