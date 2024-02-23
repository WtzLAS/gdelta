cmake_minimum_required(VERSION 3.10)
project(gdelta VERSION 2.0)

include(FetchContent)

set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

FetchContent_Declare(
    zstd
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/zstd/build/cmake/
)

FetchContent_MakeAvailable(zstd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_executable(gdelta main.cpp thirdparty/Gdelta/gdelta.cpp)

target_include_directories(gdelta PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(gdelta PUBLIC "thirdparty/argh")
target_include_directories(gdelta PUBLIC "thirdparty/Gdelta")
target_include_directories(
    gdelta
    PRIVATE
    ${zstd_SOURCE_DIR}/lib
)

target_link_libraries(
    gdelta
    PRIVATE
    libzstd_static
)
