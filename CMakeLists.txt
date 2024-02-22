cmake_minimum_required(VERSION 3.10)

project(gdelta VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(gdelta main.cpp gdelta.cpp)

configure_file(config.h.in config.h)
target_include_directories(gdelta PUBLIC "${PROJECT_BINARY_DIR}")

