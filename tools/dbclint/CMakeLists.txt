find_package(Threads REQUIRED)

add_executable(dbclint main.cpp)
target_link_libraries(dbclint cxxopts CANdb ${CMAKE_THREAD_LIBS_INIT} termcolor)
#target_link_libraries(dbclint CANdb ${CMAKE_THREAD_LIBS_INIT})
