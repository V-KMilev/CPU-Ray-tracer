cmake_minimum_required(VERSION 3.21.3)

# set the project name
project(Tutorial)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

set(SOURCES
	# App.cpp
	test.cpp
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# add the executable
add_executable(Tutorial ${SOURCES})

target_link_directories(${PROJECT_NAME} PUBLIC
	./thirdparty/GLFW/lib
)

target_include_directories(${PROJECT_NAME} PUBLIC
	./thirdparty/GLFW/include
	./thirdparty/STB/include
)

target_link_libraries(${PROJECT_NAME}
	glfw3
	OpenGL::GL
	pthread
)
