add_executable(parsingTest parsing.cpp)
target_link_libraries(parsingTest CANdbc pthread)
add_test( NAME ParsingTest COMMAND parsingTest)

