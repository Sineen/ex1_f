cmake_minimum_required(VERSION 3.6)
project(ex1_f)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -lm")

set(SOURCE_FILES
    ex1.c)

add_executable(ex1_f ${SOURCE_FILES})