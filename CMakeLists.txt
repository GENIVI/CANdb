cmake_minimum_required(VERSION 3.0.0)

project(CANdb CXX C)

option(CANDB__WITH_COVERAGE "Build with coverage" OFF)
option(CANDB__WITH_TESTS "Build with test" ON)
option(CANDB__WITH_TOOLS "Build example dbc tools" ON)
option(CANDB__TRAVIS_BUILD "Build via travis" ON)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

add_definitions(-DFMT_HEADER_ONLY) # for spdlog/libfmt
if (NOT CANDB__TRAVIS_BUILD)
    find_package(spdlog 1.4.2 REQUIRED)
    set(LIBSPDLOG spdlog::spdlog)
else (NOT CANDB__TRAVIS_BUILD)
    # only to allow building with travis... for now
    set(LIBSPDLOG spdlog)
endif (NOT CANDB__TRAVIS_BUILD)

add_subdirectory(3rdParty)

if(CANDB__WITH_COVERAGE)
    set(CMAKE_CXX_FLAGS "-g -O0 -Wwrite-strings -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS="-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
endif()

add_subdirectory(src)

if((CANDB__WITH_TESTS OR CANDB__WITH_COVERAGE))
    enable_testing()
    add_subdirectory(tests)
endif()

if(CANDB__WITH_TOOLS)
    add_subdirectory(tools)
endif(CANDB__WITH_TOOLS)

install(FILES CANdbConfig.cmake DESTINATION .)
