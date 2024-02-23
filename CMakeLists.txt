cmake_minimum_required(VERSION 3.10)
project(gdelta VERSION 2.0)

include(FetchContent)

set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)
set(ZSTD_BUILD_PROGRAMS OFF)

FetchContent_Declare(
    zstd
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/zstd
    SOURCE_SUBDIR build/cmake
)

FetchContent_MakeAvailable(zstd)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_executable(gdelta main.cpp thirdparty/Gdelta-Fork/gdelta.cpp)

if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET gdelta PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

target_include_directories(gdelta PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(gdelta PRIVATE "thirdparty/argh")
target_include_directories(gdelta PRIVATE "thirdparty/Gdelta-Fork")
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
