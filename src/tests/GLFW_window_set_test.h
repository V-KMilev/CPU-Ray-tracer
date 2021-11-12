// "#version 330 core" - using GLS (GL Shaders) version 330;
// "core" - blocking deprecated functions;
// "layout(location = n)" - Attribute is located at index n;
//
// #var - return var into char* (string);
//
// MY_GL_CHECK - macro;
//
// size - size as size in bytes
// count - size as count of elements

#pragma once

#include <iostream>

#include "GL_error_handler.h"
#include "GL_shader.h"
#include "GL_texture.h"
#include "GL_renderer.h"
#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_vertex_buffer.h"
#include "GL_vertex_buffer_layout.h"

#include "TestClearColor.h"
#include "TestTexture2D.h"

const char* gl_version = "#version 330";

int window_setup() {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Using OpenGL version 3.3.0 (major 3, minor 3) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(
		glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
		glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
		"dont open",
		glfwGetPrimaryMonitor(),    // nullptr is not fullsrean
		nullptr
	);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	// We have this scope so we don't have to make everything pointers or new and then delete
	{
		/* GL basic funtions sets*/
		MY_GL_CHECK(glEnable(GL_BLEND));
		MY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* ImGui Setup */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(gl_version);

		Renderer renderer;

		test::Test* currentTest = nullptr;
		test::TestManu* testMenu = new test::TestManu(currentTest);
		currentTest = testMenu;

		testMenu->registerTest<test::TestClearColor>("clear color");
		testMenu->registerTest<test::TestTexture2D>("texture");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {

			/* Render here */
			renderer.clear();
			renderer.clearColor();

			/* ImGui New Frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if(currentTest) {
				currentTest->onUpdate(0.0f);
				currentTest->onRender();

				ImGui::Begin("Test");
				if(currentTest != testMenu && ImGui::Button("<<")) {
					
					delete currentTest;
					currentTest = testMenu;
				}

				currentTest->onImGuiRender();
				ImGui::End();
			}

			/* ImGui Render */
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete currentTest;
		if(currentTest != testMenu) { delete testMenu; }
	}

	/* ImGui Shutdown */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
