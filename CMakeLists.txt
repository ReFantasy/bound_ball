cmake_minimum_required(VERSION 3.0)
project(bound_ball)
set(CMAKE_CXX_STANDARD 17)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARY})

IF (WIN32)
    MESSAGE(STATUS "System is windows")
    include_directories("E:/OpenGL/include")
    LINK_DIRECTORIES("E:/OpenGL/lib")
    link_libraries(optimized glew32 glfw3 soil2)
    link_libraries(debug glew32d glfw3d soil2d)
ELSEIF (APPLE)
    MESSAGE(STATUS "System is Apple.")
    include_directories("/usr/local/Cellar/glew/2.2.0_1/include")
    include_directories("/usr/local/Cellar/glfw/3.3.3/include")
    include_directories("/usr/local/Cellar/glm/0.9.9.8/include")
    link_directories("/usr/local/Cellar/glew/2.2.0_1/lib")
    link_directories("/usr/local/Cellar/glfw/3.3.3/lib")
    link_libraries(${OPENGL_LIBRARY} GLEW glfw)
ELSEIF (UNIX)
    MESSAGE(STATUS "System is UNIX-like.")
    include_directories("/usr/include")
    link_directories("/usr/lib/x86_64-linux-gnu")
    link_libraries(${OPENGL_LIBRARY} GLEW glfw)
ENDIF ()


add_executable(bound_ball main.cpp render_ball.h render_ball.cpp Sphere.h Sphere.cpp glhelper.h glhelper.cpp 
               ./shader/vertShader.glsl ./shader/fragShader.glsl)

