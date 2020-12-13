cmake_minimum_required(VERSION 2.4)

project(a_simple_experimental_database)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(db ./src/REPL.c) 
