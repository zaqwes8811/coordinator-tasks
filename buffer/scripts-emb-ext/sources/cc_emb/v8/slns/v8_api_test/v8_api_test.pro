TEMPLATE = app
CONFIG += console
CONFIG -= qt

#CONFIG += r16258
CONFIG += r10448

INCLUDEPATH += \
    ../../../../third_party/gmock-1.6.0/fused-src \
    /home/zaqwes/work/third_party \
    ../../src

LIBS += -lpthread

r16258 {
LIBS += -L/home/zaqwes/work/third_party/v8-r16258/out/ia32.release/obj.target/tools/gyp -lv8_base.ia32 -lv8_nosnapshot.ia32

}

r10448 {
LIBS += -L/home/zaqwes/work/third_party/v8-r10448 -lv8
}

SOURCES += \
    ../../src/launcher_process.cc \
    ../../../../third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc


r10448 {
SOURCES += \
    ../../src/v8-r10448/api_test.cc \
    #../../src/v8-r10448/samples/shell.cc \
    ../../src/v8-r10448/samples/process.cc \
    #../../src/v8-r10448/samples/lineprocessor.cc
}

r16258 {
SOURCES += \
    ../../src/v8-r16258/point_v8.cc \
    ../../src/v8-r16258/v8_api_test.cc \
    ../../src/v8-r16258/process.cc \
    ../../src/v8-r16258/process_test.cc \
    ../../src/v8-r16258/process_test_ext_test.cc \
    ../../src/v8-r16258/point_expose_test.cc \
    ../../src/v8-r16258/point_engine_test.cc \
    ../../src/v8-r16258/palette_engine_test.cc \
    ../../src/v8-r16258/process_test_ext.cc \
    ../../src/v8-r16258/v8_wrapper.cc \
    ../../src/v8-r16258/js_request.cc \
    ../../src/v8-r16258/virtual_machine.cc \
    ../../src/v8-r16258/v8_wrapper_db.cc \
    ../../src/v8-r16258/arrays_test.cc \
    ../../src/v8-r16258/engine_cut_persistents.cc \
    ../../src/v8-r16258/database_manipulations.cc \
    ../../src/v8-r16258/palette_v8.cc
}

HEADERS += \
    ../../src/raw/point.h \
    ../../src/raw/palette.h

