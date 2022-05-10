cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(database VERSION 1.0)

enable_testing()
include(CTest)
if(BUILD_TESTING)
	add_subdirectory(t)
endif()

add_subdirectory(src)
add_executable(REPL main.c b_tree.c disk_operation.c)

add_executable(tester main.cpp)

add_test(Tester tester)
