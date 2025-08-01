cmake_minimum_required(VERSION 3.10)
project(minidb)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(minidb ${SOURCES})

target_include_directories(minidb PRIVATE src)
