set(SRC
    dbcparser.cpp
)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/dbc_grammar.peg DBC_GRAMMAR)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dbc_grammar.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/dbc_grammar.hpp)

add_library(CANdb ${SRC} ${dbc_grammar})
target_include_directories(CANdb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(CANdb cpp-peglib spdlog)

set_target_properties(CANdb PROPERTIES CMAKE_CXX_FLAGS "-fPIC")
set_target_properties(CANdb PROPERTIES
            OUTPUT_NAME CANdb
            DEBUG_POSTFIX "d"
            RELEASE_POSTFIX ""
            RELWITHDEBINFO_POSTFIX ""
            BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS CANdb DESTINATION lib)
install(
    FILES 
        cantypes.hpp
        log.hpp
        parser.hpp
        dbcparser.h 
    DESTINATION 
        include
)


