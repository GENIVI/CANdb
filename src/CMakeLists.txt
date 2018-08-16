
embed_resources(dbc_grammar dbc_grammar.peg)
set(SRC
    dbcparser.cpp
)

add_library(CANdb ${SRC} ${dbc_grammar} dbc_grammar.peg)
target_include_directories(CANdb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CANdb cpp-peglib spdlog embed)

