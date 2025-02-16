cmake_minimum_required(VERSION 3.5)
project(EDEN VERSION 1.0)  
cmake_policy(SET CMP0072 NEW)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ensure Homebrew directories are used
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories("/opt/homebrew/include")
link_directories("/opt/homebrew/lib")

# Find Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Source Files
add_executable(EDEN 
inc/config.h 
inc/camera.h
src/camera.cpp
src/main.cpp
src/glad.c
inc/keyboard.h
src/keyboard.cpp
inc/entity.h
src/entity.cpp)

# Include directories
target_include_directories(EDEN PRIVATE dependencies inc "/opt/homebrew/include")

# Link Libraries
target_link_libraries(EDEN glfw OpenGL::GL glm)