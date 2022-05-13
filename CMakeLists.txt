cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(database VERSION 1.0)

enable_testing()
include(CTest)
if(BUILD_TESTING)
	add_subdirectory(t)
endif()

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/.*c)
add_executable(${CMAKE_CURRENT_SOURCE_DIR}/REPL ${SRC_FILES})
