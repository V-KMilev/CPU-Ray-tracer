cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

find_package(OpenMP REQUIRED)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(SOURCES
	App.cpp
	# test.cpp
)

# add the executable
add_executable(Tutorial ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)