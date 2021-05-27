if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES
        spdlog/1.8.5
        cxxopts/2.2.1
        gtest/1.10.0
        cpp-peglib/1.3.7
        tl-expected/20190710
        termcolor/1.0.1
        benchmark/1.5.2
    BASIC_SETUP CMAKE_TARGETS
)
add_library(cpp-peglib ALIAS CONAN_PKG::cpp-peglib)
add_library(spdlog ALIAS CONAN_PKG::spdlog)
add_library(cxxopts ALIAS CONAN_PKG::cxxopts)
add_library(termcolor ALIAS CONAN_PKG::termcolor)
add_library(tl-expected ALIAS CONAN_PKG::tl-expected)
add_library(gtest ALIAS CONAN_PKG::gtest)
add_library(gtest_main INTERFACE)
add_library(fmt ALIAS CONAN_PKG::fmt)
add_library(benchmark ALIAS CONAN_PKG::benchmark)
