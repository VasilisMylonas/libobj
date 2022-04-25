cmake_minimum_required(VERSION 3.0.0)
project(libobj VERSION 0.1.0)

add_library(obj obj.c)

add_executable(obj_test obj_test.c)
target_link_libraries(obj_test obj)
add_test(NAME obj_test COMMAND obj_test)
