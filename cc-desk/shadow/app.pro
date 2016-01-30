# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
# http://qt-project.org/forums/viewthread/17516

TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
OBJECTS_DIR = obj

# Try clarify messages
# http://channel9.msdn.com/Events/GoingNative/GoingNative-2012/Clang-Defending-C-from-Murphy-s-Million-Monkeys
# http://stackoverflow.com/questions/13198712/trouble-building-tests-with-google-test-clang-and-libc
# похоже бинарно стандартная либа не совместима с gcc, и поэтому в рунтайме программа падает
# clang - Seg. fault on runtime
#TARGET_EXT = .bc QMAKE_EXT_OBJ = .bc QMAKE_CXXFLAGS += -emit-llvm QMAKE_CXX = clang++
#QMAKE_CC = clang
#QMAKE_LINK = clang++
#QMAKE_LINK_SHLIB = clang++
#QMAKE_LIB = llvm-ld -link-as-library -o
#QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_CXXFLAGS += -stdlib=libstdc++  # here malloc.c failed but don't help
#
# https://www.ics.com/blog/introduction-clang#.VCPWDlSjLec - программа хотя бы запустилась, но все равно упала

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
ASL=/home/zaqwes/work/third_party/asl_1.0.43
LOCAL_3RDPARTY=../3rdparty

# -std=c++11 -pg -O2 -Wno-deprecated
QMAKE_CXXFLAGS += -Wall
# -stdlib=libc++  # http://stackoverflow.com/questions/19774778/when-is-it-necessary-to-use-use-the-flag-stdlib-libstdc
#QMAKE_CXXFLAGS += -std=c++11  # пока что, но вообще не хотелось бы

SOURCES += \
    $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
    launcher.cc

SOURCES += \
  src/canary/entities.cc \
  src/_trash/_trash.cc \
  src/canary/app_core.cc \
  src/canary/renders.cc \
  src/canary/filters.cc \
  src/canary/pq_queries.cc

SOURCES += \
  src/test_help_data.cc \
  src/canary/entities_test.cc \
  src/htable_adapt_test.cc \
  src/canary/app_core_test.cc \
  src/canary/pq_queries_test.cc

SOURCES += \
    #../../../work-analyse/hypertable/src/cc/Common/Error.cc \
    #../../../work-analyse/hypertable/src/cc/Common/Logger.cc \
    #../../../work-analyse/hypertable/src/cc/Common/String.cc \

INCLUDEPATH += \
  $$BOOST \
  $$ASL \
  $$LOCAL_3RDPARTY/loki-0.1.7/include \
  $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/ \
  src \
  /home/zaqwes/work/research_toolboxes/in-the-vicinity-cc/toolbox \
  /home/zaqwes/work/work-analyse/hypertable/src/cc
  
# -ltbb
LIBS += -lpthread -lrt -lpq -lpqxx -L/home/zaqwes/work/third_party/boost_1_55_0/stage/lib  -lboost_system

HEADERS += \
    src/canary/app_types.h \
    src/canary/lower_level.h \
    src/canary/busi.h \
    src/canary/renders.h \
    src/test_help_data.h \
    src/canary/filters.h \
    src/canary/pq_queries.h

OTHER_FILES += \
    forge.txt




