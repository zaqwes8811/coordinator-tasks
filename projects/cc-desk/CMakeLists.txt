# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#
# Trouble:
#   https://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects
#   http://stackoverflow.com/questions/19761767/qt-5-cmake-fails-with-undefined-reference-to-vtable-on-hello-world-with-inc-sr

project(coordinator_tasks)
cmake_minimum_required(VERSION 2.8)

# http://www.qtcentre.org/wiki/?title=Compiling_Qt4_apps_with_CMake
# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
SET( QT_USE_QT3SUPPORT TRUE )
SET( QT_USE_QTXML TRUE )

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )

set( LCB_USE_ASAN Y)

set(Extern "../../extern/")
set(ExternLocal "./extern/")
set(Export "../../export/")
set(GMock "../../extern/gmock-1.6.0/fused-src")
set(Source "source/")
set(testsRoot "./tests")
set(root ".")
set(BigExtern "/opt/big-3rdparty/")
set(Intern "../../intern/")

# Test framework
include_directories(${GMock})
include_directories(${Source})
include_directories(${Extern}/loki-0.1.7/include)
include_directories(${Extern})
include_directories(${ExternLocal})
include_directories(${Source}/../../std_own_ext)
#include_directories(${BigExtern}/boost_1_57_0)

# ASan,TSan and MSan
IF(LCB_USE_ASAN)
    # GCC: troubles with symbols
    #ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer-3.4)
    #/usr/bin/llvm-symbolizer-3.4

    # TOOLS:
    #
    # https://code.google.com/p/address-sanitizer/wiki/HowToBuild
    # http://cameo54321.blogspot.ru/2014/02/installing-cmake-288-or-higher-on.html
    # clang 3.1 >= need for ASan Ub12.04 -> 3.0
    #
    # !!https://solarianprogrammer.com/2013/01/17/building-clang-libcpp-ubuntu-linux/
    # Try it! https://github.com/pacs-course/pacs/wiki/Instructions-to-install-clang-3.1-on-ubuntu-12.04.1-and-12.10
    # http://stackoverflow.com/questions/17657261/how-to-install-clang-pre-built-binaries-ubuntu-12-04
    # http://sohanikh.blogspot.ru/2013/02/how-to-build-llvm-from-source-on-ubuntu.html
    #add_definitions(  -Wall -g -fsanitize=address)
    # https://developer.mozilla.org/en-US/docs/Mozilla/Testing/Firefox_and_Address_Sanitizer

    # Static analyser http://clang-analyzer.llvm.org/
    # https://github.com/couchbase/couchnode/blob/master/deps/lcb/cmake/Modules/GetLibcouchbaseFlags.cmake
    # http://blogs.perl.org/users/rurban/2011/11/adventures-with-clang-and-asan.html
    # main: address - thread, memory and ub not work in i686, only x86_64
    #-fno-omit-frame-pointer -O1 -g
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fno-omit-frame-pointer -fsanitize=address")
    #-fno-omit-frame-pointer -O1
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fsanitize=address")
    # -undefined dynamic_lookup
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=address")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
ENDIF()

# link the libraries to the executable
if (MSVC90)
    # FIXME: C runtime troubles
    # http://stackoverflow.com/questions/14172856/cmake-compile-with-mt-instead-of-md
    # http://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx
    # http://stackoverflow.com/questions/757418/should-i-compile-with-md-or-mt
    #
    # http://rsdn.ru/forum/cpp.applied/2843738.hot
    # /MT
    #link_directories(${BigExtern}/boost_1_57_0/stage/lib)
    add_definitions( -D_CRT_SECURE_NO_WARNINGS)# /showIncludes)#D)#T)
    #set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    LINK_LIBRARIES(ws2_32 WSnmp32 libboost_system-vc90-mt-1_57)# libboost_thread-vc90-mt-s-1_57.lib)
elseif (UNIX)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wconversion")


    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        link_libraries(
            pthread rt)  # !! pthread and rt last!!
    else()
        #link_directories(${BigExtern}/boost_1_57_0/stage_x64/lib)
        link_libraries(
            pqxx pq
            sqlite3
            #boost_system boost_thread
            pthread rt)  # !! pthread and rt last!!
    endif()
endif()

# some .ui files
SET( SAMPLE_UIS
   ${Source}/view/mainwindow.ui
)

SET( SAMPLE_MOC_HDRS
   ${Source}/view/mainwindow.h
)

QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )

# this will run uic on .ui files:
QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

file( GLOB proTests
    ${Source}/view/*.cc
    ${ExternLocal}/actors_and_workers/*.cc
    ${ExternLocal}/actors_and_workers/*.h
    ${ExternLocal}/std_own_ext-fix/*.h
    ${Source}/../../std_own_ext/*.h
    ${ExternLocal}/data_access_layer/*.cc
    ${ExternLocal}/data_access_layer/*.h
    ${Source}/heart/*.h
    ${Source}/common/*.h
    ${Source}/model_layer/*.cc
    ${Source}/model_layer/*.h
    ${ExternLocal}/sqlite_xx/*.cc
    ${testsRoot}/*.cc
    ${testsRoot}/db/*.cc
)

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
# works just like INCLUDEPATH from qmake)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

add_executable(unittest.bin
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${proTests}
    ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS}
)

TARGET_LINK_LIBRARIES( unittest.bin ${QT_LIBRARIES} )

