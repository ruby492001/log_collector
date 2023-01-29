project( profile )


set( SRC_LIST
        implementation/profiler.cpp)

add_library( ${PROJECT_NAME} ${SRC_LIST} interface )

target_include_directories( ${PROJECT_NAME} PUBLIC interface )

target_link_libraries(
        ${PROJECT_NAME}
)


