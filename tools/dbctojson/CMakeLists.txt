add_executable(dbctojson main.cpp)
target_link_libraries(dbctojson cxxopts CANdbc pthread)
