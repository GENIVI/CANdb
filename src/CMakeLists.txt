set(
  SRC
  dbcparser.cpp
  parsererror.cpp
  stringutils.cpp
)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/dbc_grammar.peg DBC_GRAMMAR)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/dbc_grammar.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/dbc_grammar.hpp
)

add_library(CANdb ${SRC} ${dbc_grammar})
target_include_directories(
  CANdb
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(CANdb PUBLIC cpp-peglib spdlog tl-expected fmt)
#target_link_libraries(CANdb PUBLIC spdlog fmt)
target_compile_features(CANdb PUBLIC cxx_std_17)
