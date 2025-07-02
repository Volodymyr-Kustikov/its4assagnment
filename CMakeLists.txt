cmake_minimum_required(VERSION 3.31)
project(notionSecondEdition)

set(CMAKE_CXX_STANDARD 20)

add_executable(notionSecondEdition main.c
        cpp/additionalFunctionallity.cpp
        main.h)
