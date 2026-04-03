cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(molecular_simulation)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/data" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SFML" "${CMAKE_CURRENT_BINARY_DIR}/sfml_build")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/TGUI" "${CMAKE_CURRENT_BINARY_DIR}/tgui_build")
set(TGUI_BACKEND SFML_GRAPHICS)

add_executable(MyProgram src/main.cpp src/engine.cpp)

target_link_libraries(MyProgram sfml-graphics sfml-window sfml-system sfml-audio TGUI::TGUI)

add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}/")