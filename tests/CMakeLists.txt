add_executable(parsingTest parsing.cpp)
target_link_libraries(parsingTest CANdbc)
add_test( NAME ParsingTest COMMAND parsingTest)

