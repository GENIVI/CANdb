add_executable(dbconverter main.cpp vsi_serializer.cpp)
target_link_libraries(dbconverter cxxopts CANdb pthread)
