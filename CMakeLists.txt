cmake_minimum_required(VERSION 2.4)

project(a_simple_experimental_database)
include_directories(${PROJECT_SOURCE_DIR})

enable_testing()
add_executable(testing ./Testing/test.cpp)

target_link_libraries(testing gtest_main)

add_test(NAME testing COMMAND testing)

add_executable(db ./src/REPL.c) 
