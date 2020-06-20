add_subdirectory(termcolor)
add_subdirectory(gtest)
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "")
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(cxxopts)

add_library(cpp-peglib INTERFACE)
target_include_directories(cpp-peglib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp-peglib)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)
