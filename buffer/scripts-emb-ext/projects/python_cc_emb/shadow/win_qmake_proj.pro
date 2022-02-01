TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += static

# C
#QMAKE_CFLAGS_RELEASE -= -MD
#QMAKE_CFLAGS_RELEASE = -MT
#QMAKE_CFLAGS_DEBUG -= -MDd
#QMAKE_CFLAGS_DEBUG = -MTd

# C++
#QMAKE_CXXFLAGS_RELEASE -= -MD
#QMAKE_CXXFLAGS_RELEASE += -MT
#QMAKE_CXXFLAGS_DEBUG -= -MDd
#QMAKE_CXXFLAGS_DEBUG += -MTd

SOURCES += \
    ../../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
    ../src/tests-launcher.cc \
    ../src/expose_test.cc

INCLUDEPATH += "D:/home/lugansky-igor/in-the-vicinity-cc/third_party/gmock-1.6.0/fused-src" \
  "D:/home/third_party/cc/boost_1_47_0"
INCLUDEPATH += "C:/Python27/include"


LIBS += C:/Python27/libs/python27.lib
LIBS += "D:\home\third_party\cc\boost.python-libs/boost_python-vc100-mt-gd-1_47.lib"

HEADERS += \
    ../src/foo.h \
    ../src/foo_py.h

