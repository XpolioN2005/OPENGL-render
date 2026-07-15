cmake_minimum_required(VERSION 3.20)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGL
    src/main.cpp
    src/gl.c
)

target_include_directories(OpenGL PRIVATE include)

target_link_libraries(OpenGL PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    opengl32
)