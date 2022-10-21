


set(CMAKE_CXX_STANDARD 11)

add_executable(sqltest sqltest.cpp)
target_link_libraries(sqltest /usr/local/pgsql/lib/libpq.so ${TARGET})
target_include_directories(sqltest PUBLIC ../..)