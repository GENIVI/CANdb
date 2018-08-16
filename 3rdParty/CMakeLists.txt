add_subdirectory(embed-resource)
add_subdirectory(termcolor)
add_subdirectory(gtest)
set(CXXOPTS_BUILD_EXAMPLES CACHE FORCE OFF)
add_subdirectory(cxxopts)

add_library(cpp-peglib INTERFACE)
target_include_directories(cpp-peglib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp-peglib)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

add_library(embed INTERFACE)
target_include_directories(embed INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/embed-resource)
