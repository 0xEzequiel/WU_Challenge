cmake_minimum_required(VERSION 3.10)
project(TestSuite)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests tests.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

add_definitions(-DTESTING)