cmake_minimum_required(VERSION 3.8.0)

project(CANdb CXX C)

option(CANDB__WITH_COVERAGE "Build with coverage" ON)
option(CANDB__WITH_TESTS "Build with test" ON)
option(CANDB__WITH_TOOLS "Build example dbc tools" ON)
option(CANDB__WITH_CONAN "3rdParty with conan " OFF)

if(CANDB__WITH_CONAN)
    include(cmake/Conan.cmake)
else()
    add_subdirectory(3rdParty)
    add_library(fmt INTERFACE)
endif()

include(cmake/code-coverage.cmake)
add_code_coverage()
add_code_coverage_all_targets()

add_subdirectory(src)

if(CANDB__WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CANDB__WITH_TOOLS)
  add_subdirectory(tools)
endif(CANDB__WITH_TOOLS)
