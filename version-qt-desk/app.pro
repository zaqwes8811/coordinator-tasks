# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file

TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
OBJECTS_DIR = obj

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
ASL=/home/zaqwes/work/third_party/asl_1.0.43
LOCAL_3RDPARTY=../3rdparty

# -std=c++11 -pg
QMAKE_CXXFLAGS += -O2  -Wall -Wno-deprecated 
# -stdlib=libc++  # http://stackoverflow.com/questions/19774778/when-is-it-necessary-to-use-use-the-flag-stdlib-libstdc
QMAKE_CXXFLAGS += -std=c++11  # пока что, но вообще не хотелось бы

SOURCES += \
    src/canary/pq_dal_test.cc \
    src/canary/pq_dal.cc \
    $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
    launcher.cc \
    src/canary/entities.cc \
    src/canary/entities_test.cc \
    src/_trash/_trash.cc \
    src/htable_adapt_test.cc \
    ../../../work-analyse/hypertable/src/cc/Common/Error.cc \
    ../../../work-analyse/hypertable/src/cc/Common/Logger.cc \
    ../../../work-analyse/hypertable/src/cc/Common/String.cc \
    src/canary/app_core.cc \
    src/canary/app_core_test.cc \
    src/canary/renders.cc

INCLUDEPATH += \
  $$BOOST \
  $$ASL \
  $$LOCAL_3RDPARTY/loki-0.1.7/include \
  $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/ \
  src \
  /home/zaqwes/work/research_toolboxes/in-the-vicinity-cc/toolbox \
  /home/zaqwes/work/work-analyse/hypertable/src/cc
  
# -ltbb
LIBS += -lpthread -lrt  -lpq -lpqxx -L/home/zaqwes/work/third_party/boost_1_55_0/stage/lib  -lboost_system

HEADERS += \
    src/canary/app_types.h \
    src/canary/lower_level.h \
    src/canary/busi.h \
    src/canary/renders.h

OTHER_FILES += \
    forge.txt




