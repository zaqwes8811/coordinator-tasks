# http://www.zaita.com/2013/06/a-simple-and-reusable-cmake-file/
#
cmake_minimum_required(VERSION 2.8.4)
project(version_qt_desk)

set(LOCAL_3RD_PARTY "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty")
set(_3RD_PARYT "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/gmock-1.6.0/fused-src")
set(TARGET_APP a.app)

set(BOOST_, "/home/zaqwes/work/third_party/boost_1_55_0")
set(ASL_, "/home/zaqwes/work/third_party/asl_1.0.43")  # doesn't work

include_directories("/home/zaqwes/work/third_party/boost_1_55_0")
include_directories("/home/zaqwes/work/third_party/asl_1.0.43")
#include_directories(${ASL_})  #"/home/zaqwes/work/third_party/asl_1.0.43")
include_directories(${_3RD_PARYT})
include_directories(${LOCAL_3RD_PARTY}/loki-0.1.7/include)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")


# SHARED STATIC
# странно как-то находит библиотеку.
add_library(gtest SHARED ${_3RD_PARYT}/gmock-gtest-all.cc)


file(
    GLOB
    src
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

file(
    GLOB
    rest
    "${CMAKE_CURRENT_SOURCE_DIR}/src/canary/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

add_executable("${TARGET_APP}" "${src}" ${rest})

target_link_libraries("${TARGET_APP}" gtest)
target_link_libraries("${TARGET_APP}" pthread)
target_link_libraries("${TARGET_APP}" pq)
target_link_libraries("${TARGET_APP}" pqxx)
# -L/home/zaqwes/work/third_party/boost_1_55_0/stage/lib  -lboost_system  # not need?
