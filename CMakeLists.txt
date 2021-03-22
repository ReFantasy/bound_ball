cmake_minimum_required(VERSION 3.0)
project(bound_ball)
set(CMAKE_CXX_STANDARD 14)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARY})


include_directories("E:/OpenGL/include")


LINK_DIRECTORIES("E:/OpenGL/lib")

link_libraries(optimized glew32 glfw3 soil2)
link_libraries(debug glew32d glfw3d soil2d)

add_executable(bound_ball main.cpp vertShader.glsl fragShader.glsl)

