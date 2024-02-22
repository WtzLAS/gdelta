cmake_minimum_required(VERSION 3.10)

project(gdelta VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_executable(gdelta main.cpp thirdparty/Gdelta/gdelta.cpp)

target_include_directories(gdelta PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(gdelta PUBLIC "thirdparty/argh")
target_include_directories(gdelta PUBLIC "thirdparty/Gdelta")
