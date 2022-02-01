cmake_minimum_required(VERSION 2.8)

project(hello_world)

set(SOURCE_EXE main.cpp)

include_directories(
    "D:/home/work-libs/cpp/gmock-1.6.0/fused-src")

set(SOURCE
    D:/home/work-libs/cpp/gmock-1.6.0/fused-src/gmock-gtest-all.cc
    foo.cpp)

add_library(sources STATIC ${SOURCE})
add_executable(main ${SOURCE_EXE})
target_link_libraries(main sources)

