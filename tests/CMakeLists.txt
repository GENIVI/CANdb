add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(dbcparser_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)
add_test(NAME dbcparser_tests COMMAND dbcparser_tests)

add_executable(opendbc_tests opedbc_tests.cpp)
target_link_libraries(opendbc_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_compile_definitions(opendbc_tests PRIVATE OPENDBC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/dbc/opendbc/")
target_include_directories(opendbc_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)
add_test(NAME opendbc_tests COMMAND opendbc_tests)

find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind
        COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ctest)
endif()
