cmake_minimum_required(VERSION 3.17)

project(dipc)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} STATIC
    src/dipc.cpp
    )

target_include_directories(${PROJECT_NAME} PRIVATE include)