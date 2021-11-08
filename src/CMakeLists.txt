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

set(IMGUI_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui"
)

set(IMGUI
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
	"${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"

	"${IMGUI_DIR}/backends/imgui_impl_glfw.h"
	"${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"

	"${IMGUI_DIR}/imgui_demo.cpp"
	"${IMGUI_DIR}/imgui_draw.cpp"
	"${IMGUI_DIR}/imgui_tables.cpp"
	"${IMGUI_DIR}/imgui_widgets.cpp"

	"${IMGUI_DIR}/imgui.h"
	"${IMGUI_DIR}/imgui.cpp"
)

set(GLM
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm/glm"
)

message(INFO " ========================================= ${GLAD_GL}")

set(SOURCES
	${GLAD_GL}
	${IMGUI}
	${GLM}

	# App.cpp
	Test.cpp
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# add executables
add_executable(Tutorial ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/STB/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include"
	"${GLFW_SOURCE_DIR}/deps"
	"${IMGUI_DIR}"
	"${IMGUI_DIR}/backends"
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm/glm"
)

target_link_libraries(${PROJECT_NAME} PUBLIC # Don't change the postions of the libraries!
	glfw
	OpenGL::GL
	${CMAKE_DL_LIBS}
)
