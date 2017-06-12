add_executable(parsingTest parsing.cpp)
target_link_libraries(parsingTest CANdbc ${CMAKE_THREAD_LIBS_INIT})
add_test( NAME ParsingTest COMMAND parsingTest)

