project(SUBD)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        main.c
        String/myString.c)

add_executable(SUBD ${SOURCES})
