
cmake_minimum_required(VERSION 3.19)
project(try_ovm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(OpenVolumeMesh)

add_executable(try_ovm src/main.cpp)

target_link_libraries(try_ovm OpenVolumeMesh)