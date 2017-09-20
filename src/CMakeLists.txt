
embed_resources(dbc_grammar dbc_grammar.peg)
set(SRC
    candb.cpp
    dbcparser.cpp
)

add_library(CANdbc ${SRC} ${dbc_grammar} dbc_grammar.peg)
target_include_directories(CANdbc PRIVATE ${CMAKE_SOURCE_DIR}/3rdParty/cpp-peglib/)

