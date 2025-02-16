cmake_minimum_required(VERSION 3.5)
project(EDEN VERSION 1.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(IMGUI_SOURCE ${CMAKE_MODULE_PATH}
                 ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)

# Add ImGui source files
set(IMGUI_SOURCES
    dependencies/imgui/imgui.cpp
    dependencies/imgui/imgui_demo.cpp
    dependencies/imgui/imgui_draw.cpp
    dependencies/imgui/imgui_widgets.cpp
    dependencies/imgui/imgui_impl_opengl3.cpp
    dependencies/imgui/imgui_impl_glfw.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/implot.cpp
    dependencies/imgui/implot_demo.cpp
    dependencies/imgui/implot_items.cpp)

set(EDEN_TOOL_SOURCES src/eden_tools.cpp)

add_executable(EDEN src/config.h src/main.cpp src/glad.c ${IMGUI_SOURCES}
                    ${EDEN_TOOL_SOURCES})

target_include_directories(EDEN PRIVATE dependencies)

target_link_libraries(EDEN ${GLFW3_LIBRARY} OpenGL::GL glfw)
