add_executable(dbclint main.cpp)
target_link_libraries(dbclint cxxopts CANdb pthread termcolor)
