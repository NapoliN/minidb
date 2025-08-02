cmake_minimum_required(VERSION 3.10)
project(minidb)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(minidb ${SOURCES})

target_include_directories(minidb PRIVATE src)

add_subdirectory(external/parsertl17)
target_link_libraries(minidb PRIVATE parsertl)

add_subdirectory(external/lexertl17)
target_link_libraries(minidb PRIVATE lexertl)