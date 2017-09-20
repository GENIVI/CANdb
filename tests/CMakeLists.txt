include(GoogleTest)

add_executable(dbcparser_tests dbcparser_tests.cpp)
target_link_libraries(dbcparser_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(dbcparser_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

gtest_add_tests( dbcparser_tests "" AUTO)

add_executable(opendbc_tests opedbc_tests.cpp)
target_link_libraries(opendbc_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_compile_definitions(opendbc_tests PRIVATE OPENDBC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/dbc/opendbc/")
target_include_directories(opendbc_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)
gtest_add_tests( opendbc_tests "" AUTO)

add_executable(peg_grammar_tests peg_grammar_tests.cpp)
target_link_libraries(peg_grammar_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(peg_grammar_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

gtest_add_tests( peg_grammar_tests "" AUTO)

add_executable(tesla_dbc_tests tesla_dbc_tests.cpp)
target_link_libraries(tesla_dbc_tests CANdbc ${CMAKE_THREAD_LIBS_INIT} gtest gtest_main)
target_include_directories(tesla_dbc_tests PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

gtest_add_tests( tesla_dbc_tests "" AUTO)
