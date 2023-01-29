project( event_generator )


set( SRC_LIST
        implementation/event_generator.cpp)

add_library( ${PROJECT_NAME} ${SRC_LIST} interface )

target_include_directories( ${PROJECT_NAME} PUBLIC interface )

target_link_libraries(
        ${PROJECT_NAME}
        event
        event_list
)


