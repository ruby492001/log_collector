project(testbin1)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set( Boost_USE_STATIC_LIBS OFF )

set(SRC_LIST
        implementation/testbin1.cpp
        )

include_directories(${Boost_INCLUDE_DIR})
add_executable( ${PROJECT_NAME} ${SRC_LIST} )

target_include_directories(${PROJECT_NAME} PUBLIC interface)

target_link_libraries(
    ${PROJECT_NAME}
    event
    event_list
    event_generator
    db_wrapper_sqlite
    db_wrapper_mysql
    db_wrapper_postgres
    soci_postgresql
    dl
    pq
    db_wrapper_itf
)
