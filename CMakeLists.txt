cmake_minimum_required(VERSION 3.5)
project(EDEN VERSION 1.0)  
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(EDEN 
    inc/config.h 
    inc/camera.h
    src/camera.cpp
    src/main.cpp
    src/glad.c)

target_include_directories(EDEN PRIVATE dependencies inc)

target_link_libraries(EDEN ${GLFW3_LIBRARY} OpenGL::GL)