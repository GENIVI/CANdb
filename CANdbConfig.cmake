if (CANdb_FOUND)
    return()
endif(CANdb_FOUND)

set (CANDB_VERSION ${CANDB_VERSION_MAJOR}${CANDB_VERSION_MINOR}0)
set (CANdb_LIB "CANdb")

include_directories(${CANdb_DIR}/include)
link_directories(${CANdb_DIR}/lib)

add_library(${CANdb_LIB} STATIC IMPORTED)

set_target_properties(${CANdb_LIB} PROPERTIES
    IMPORTED_LOCATION_DEBUG ${CANdb_DIR}/lib/lib${CANdb_LIB}d.a
    IMPORTED_LOCATION_RELEASE ${CANdb_DIR}/lib/lib${CANdb_LIB}.a
    IMPORTED_LOCATION_RELWITHDEBINFO ${CANdb_DIR}/lib/lib${CANdb_LIB}.a
)
