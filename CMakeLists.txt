cmake_minimum_required(VERSION 3.8.2)

project(CANdbc CXX C)

option(WITH_COVERAGE "Build with coverage" OFF)
option(WITH_TESTS "Build with test" ON)
option(WITH_TOOLS "Build example dbc tools" ON)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/embed-resource)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/cereal/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/embed-resource)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/termcolor)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/gtest)
set(CXXOPTS_BUILD_EXAMPLES CACHE FORCE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/cxxopts)

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

if(WITH_TOOLS)
    add_subdirectory(tools)
endif(WITH_TOOLS)
