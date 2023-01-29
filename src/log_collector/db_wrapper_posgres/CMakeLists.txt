project( db_wrapper_postgres )


set( SRC_LIST
        implementation/db_wrapper_postgres.cpp)

add_library( ${PROJECT_NAME} ${SRC_LIST} interface )

target_include_directories( ${PROJECT_NAME} PUBLIC interface )

target_link_libraries(
        ${PROJECT_NAME}
        db_wrapper_itf
        soci_core
        soci_postgresql
        dl
        pq
        profile
)


