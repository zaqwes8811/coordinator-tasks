# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_BUILD_TYPE=Release
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

# FIXME: use TSan
# Keys is exclude other
set( LCB_USE_ASAN Y)
#set( LCB_USE_TSAN Y)

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
include_directories(${BigExtern}/boost_1_57_0)
include_directories(/opt/folly/include)

# ASan,TSan and MSan
IF(LCB_USE_ASAN)
    #-fno-omit-frame-pointer -O1 -g
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fno-omit-frame-pointer -fsanitize=address")
    #-fno-omit-frame-pointer -O1
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fno-omit-frame-pointer -fsanitize=address")
    # -undefined dynamic_lookup
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=address")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
ENDIF()

IF(LCB_USE_TSAN)
    # http://stackoverflow.com/questions/26507205/error-when-attempting-to-compile-libpqxx-4-0-1-on-solaris-10-using-oracle-solari
    # Trouble with libpqxx - need compile with -fPIC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fsanitize=thread -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fsanitize=thread -fPIC")
    #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -pie")

    #link_directories(${BigExtern}/boost_1_57_0/gcc_tsan_lib64/lib)
endif()

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
        link_directories(${BigExtern}/boost_1_57_0/stage_x64/lib)
        link_directories(${BigExtern}/../folly/lib)
        link_libraries(
            pqxx pq
            sqlite3
            folly glog boost_system boost_thread  # FIXME: it's bad! Just try interface. Watch Sean Parent
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

file( GLOB src
    ${Source}/view/*.cc
    ${Source}/core/*.cc
    ${Source}/core/*.h
    ${ExternLocal}/actors_and_workers/*.cc
    ${ExternLocal}/actors_and_workers/*.h
    ${ExternLocal}/data_access_layer/migration_schemas/*.h
    ${ExternLocal}/std_own_ext-fix/*.h
    ${Source}/../../std_own_ext/*.h
    ${ExternLocal}/data_access_layer/*.cc
    ${ExternLocal}/data_access_layer/*.h
    ${Source}/heart/*.h
    ${Source}/common/*.h
    ${Source}/model_layer/*.cc
    ${Source}/model_layer/*.h
    ${ExternLocal}/sqlite_xx/*.cc
)

file( GLOB tests
    ${testsRoot}/*.cc
    ${testsRoot}/queries/*.cc
    ${testsRoot}/units/*.cc
    ${testsRoot}/extern/*.cc
    ${ExternLocal}/peoples_tricks/*.cc
)

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
# works just like INCLUDEPATH from qmake)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

add_executable(unittest
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${src}
    ${tests}
    ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS}
)

add_executable(planner
    ${Source}/mains/main.cc
    ${src}
    ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS}
)

TARGET_LINK_LIBRARIES( unittest ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( planner ${QT_LIBRARIES} )

