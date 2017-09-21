add_executable(dbctojson main.cpp vsi_serializer.cpp)
target_link_libraries(dbctojson cxxopts CANdbc pthread)
