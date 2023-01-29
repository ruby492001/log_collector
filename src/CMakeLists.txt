cmake_minimum_required( VERSION 3.2 )
project(log_collector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#add_subdirectory(log_collector/soci_db)
add_subdirectory( log_collector/db_wrapper_itf )
add_subdirectory( log_collector/db_wrapper_sqlite )
add_subdirectory( log_collector/db_wrapper_mysql )
add_subdirectory( log_collector/db_wrapper_posgres )
add_subdirectory( log_collector/event )
add_subdirectory( log_collector/event_list )
add_subdirectory( log_collector/testbin1 )
add_subdirectory( log_collector/event_generator )
add_subdirectory( log_collector/profiler )

