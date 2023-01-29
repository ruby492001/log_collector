project( db_wrapper_itf )

set( SRC_LIST
        implementation/db_wrapper_itf.cpp )

add_library( ${PROJECT_NAME} ${SRC_LIST} interface )

target_include_directories( ${PROJECT_NAME} INTERFACE interface )

target_link_libraries(
    ${PROJECT_NAME}
    event_list
    event
    soci_core
    soci_sqlite3
)




