cmake_minimum_required(VERSION 3.20)

project(OpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(OpenGL ${SOURCE_FILES}
    src/shader/ShaderParser.cpp
    src/shader/Shader.cpp)

target_include_directories(OpenGL PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(OpenGL PRIVATE
    "${CMAKE_SOURCE_DIR}/lib/glfw3.lib"
    opengl32
)