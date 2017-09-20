cmake_minimum_required(VERSION 3.9.0)

project(CANdbc CXX C)

option(WITH_COVERAGE "Build with coverage" OFF)
option(WITH_TESTS "Build with test" ON)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/embed-resource)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/embed-resource)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/gtest)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/lint)

find_package(Threads REQUIRED)

if(WITH_COVERAGE)
    set(CMAKE_CXX_FLAGS "-g -O0 -Wwrite-strings -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS="-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
endif()

add_subdirectory(src)

if((WITH_TESTS OR WITH_COVERAGE))
    enable_testing()
    add_subdirectory(tests)
endif()
