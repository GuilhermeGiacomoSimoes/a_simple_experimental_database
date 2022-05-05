cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(database VERSION 1.0)

add_subdirectory(src)

enable_testing()

add_subdirectory(t)
