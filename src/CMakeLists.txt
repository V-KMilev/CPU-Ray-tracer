cmake_minimum_required(VERSION 3.21.3)

# set the project name
project(Tutorial)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

set(SOURCES
	App.cpp
	# test.cpp
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# add the executable
add_executable(Tutorial ${SOURCES})

add_subdirectory( 
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	./thirdparty/STB/include
	./thirdparty/glfw/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC # Don't change the postions of the libraries!!!
	glfw
	${CMAKE_DL_LIBS}
	OpenGL::GL
)
