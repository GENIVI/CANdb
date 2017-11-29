add_executable(dbclint main.cpp)
target_link_libraries(dbclint cxxopts CANdbc pthread termcolor)
