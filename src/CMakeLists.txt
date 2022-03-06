cmake_minimum_required(VERSION 3.21.3)

# Set the project name
project(Tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(EMBREE_ISPC_SUPPORT OFF)
set(EMBREE_TUTORIALS OFF)
set(EMBREE_TUTORIALS_GLFW OFF)
set(EMBREE_TASKING_SYSTEM "INTERNAL")

# Need to have GLFW
add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw"
)
# Need to have EMBREE
add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/embree"
)

# Submodules DIRS
set(EMBREE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/embree"
)
set(GLFW_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw"
)
set(GLM_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm"
)
set(IMGUI_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui"
)

# My folders DIRS
set(MY_DEPS_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/deps"
)
set(MY_EMBREE_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/myEmbree"
)
set(MY_GL_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/myGL"
)
set(MY_GLFW_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/myGlfw"
)
set(MY_IMGUI_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/myImGui"
)
set(MY_SHADERS_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/Shaders"
)
set(MY_TEXTURES_DIR
	"${CMAKE_CURRENT_SOURCE_DIR}/Textures"
)

# Need in SOURCES files
set(EMBREE
	"${EMBREE_DIR}/include"
)
set(GLAD_GL
	"${GLFW_DIR}/deps/glad/gl.h"
	"${GLFW_DIR}/deps/glad_gl.c"
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
	"${GLM_DIR}/glm"
)

# INFO
message(
INFO
" ==================================
-- ${EMBREE}
-- ${GLAD_GL}
-- ${GLM}
-- ${IMGUI}\n"
INFO
" =================================="
)

set(SOURCES
	${GLAD_GL}
	${IMGUI}
	${GLM}

	# App.cpp
	Test.cpp
)

# add executables
add_executable(Tutorial ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	"${EMBREE_DIR}/include"

	"${GLFW_DIR}/include"
	"${GLFW_DIR}/deps"

	"${GLM_DIR}/glm"

	"${IMGUI_DIR}"
	"${IMGUI_DIR}/backends"

	"${MY_DEPS_DIR}"
	"${MY_DEPS_DIR}/common"
	"${MY_DEPS_DIR}/common/file_edit"
	"${MY_DEPS_DIR}/geometry"
	"${MY_DEPS_DIR}/geometry/materials"
	"${MY_DEPS_DIR}/geometry/objects"
	"${MY_DEPS_DIR}/math"
	"${MY_DEPS_DIR}/render"
	"${MY_DEPS_DIR}/scene"

	"${MY_EMBREE_DIR}"
	"${MY_GLFW_DIR}"
	"${MY_IMGUI_DIR}"
	"${MY_GL_DIR}"
	"${MY_SHADERS_DIR}"
	"${MY_TEXTURES_DIR}"

	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/STB/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/TOB/include"
)

# DO NOT change the postions of the libraries!
target_link_libraries(${PROJECT_NAME} PUBLIC
	embree
	glfw
	OpenGL::GL
	${CMAKE_DL_LIBS}
)
