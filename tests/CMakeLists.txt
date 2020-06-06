function(candb_add_test test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} CANdb gtest gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

candb_add_test(dbcparser_tests)
candb_add_test(opendbc_tests)

target_compile_definitions(opendbc_tests PRIVATE OPENDBC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/dbc/opendbc/")

find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind
        COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ctest)
endif()
