cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(molecular_simulation)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SFML" "${CMAKE_CURRENT_BINARY_DIR}/sfml_build")

add_executable(MyProgram src/main.cpp)

target_link_libraries(MyProgram PRIVATE SFML::Graphics)