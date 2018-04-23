
embed_resources(dbc_grammar dbc_grammar.peg)
set(SRC
    dbcparser.cpp
)

add_library(CANdbc ${SRC} ${dbc_grammar} dbc_grammar.peg)
target_include_directories(CANdbc PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/cpp-peglib/
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/embed-resource
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/spdlog/include
)

