cmake_minimum_required(VERSION 3.21.3)

# set the project name
project(Tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw"
)
	
set(GLAD_GL
	"${GLFW_SOURCE_DIR}/deps/glad/gl.h"
	"${GLFW_SOURCE_DIR}/deps/glad_gl.c"
)

message(INFO "========================================= ${GLAD_GL}")

set(SOURCES
	# App.cpp
	test.cpp
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# add the executable
add_executable(Tutorial ${SOURCES} ${GLAD_GL})

target_include_directories(${PROJECT_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/STB/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include"
	"${GLFW_SOURCE_DIR}/deps"
)

target_link_libraries(${PROJECT_NAME} PUBLIC # Don't change the postions of the libraries!
	glfw
	OpenGL::GL
	${CMAKE_DL_LIBS}
)
