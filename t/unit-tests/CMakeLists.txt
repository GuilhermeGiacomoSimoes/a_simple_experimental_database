enable_testing()
add_executable(main main.cpp)
add_test(NAME main COMMAND testDriver)
