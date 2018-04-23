add_executable(dbclint main.cpp)
target_link_libraries(dbclint cxxopts CANdbc pthread termcolor)
target_include_directories(dbclint PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/spdlog/include  ${CMAKE_CURRNENT_SOURCE_DIR}../../3rdParty/embed-resource ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
