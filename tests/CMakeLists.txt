include(GoogleTest)
function(candb_add_test test_name)
  add_executable(${test_name} ${test_name}.cpp dbc_parser_data.hpp dbc_parser_data_win.hpp)
  gtest_discover_tests(${test_name})
  target_link_libraries(${test_name} CANdb gtest gtest_main test_helper)
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_library(test_helper STATIC test_helper.cpp)
target_link_libraries(test_helper PUBLIC spdlog)
target_compile_definitions(test_helper PUBLIC DBC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/dbc/")

# check if c++17 filesystem is in place
try_compile(
  HAS_STD_FILESYSTEM "${CMAKE_BINARY_DIR}/temp" "${CMAKE_SOURCE_DIR}/cmake/has_filesystem.cpp"
  CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
)

if(NOT HAS_STD_FILESYSTEM)
  find_package(Boost REQUIRED COMPONENTS filesystem system)
  target_link_libraries(test_helper PUBLIC Boost::filesystem)
else()
  target_compile_features(test_helper PUBLIC cxx_std_17)
  target_compile_definitions(test_helper PUBLIC HAS_STD_FILESYSTEM)
endif()


candb_add_test(dbcparser_tests)
candb_add_test(opendbc_tests)
candb_add_test(prop_tests)


find_program(VALGRIND "valgrind")
if(VALGRIND)
  add_custom_target(
    valgrind
    COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ctest
  )
endif()
