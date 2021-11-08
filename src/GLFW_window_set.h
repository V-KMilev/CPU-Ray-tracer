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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "GL_error_handler.h"
#include "GL_shader.h"
#include "GL_texture.h"
#include "GL_renderer.h"
#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_vertex_buffer.h"
#include "GL_vertex_buffer_layout.h"

#include "Utility_functions.h"

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
	window = glfwCreateWindow(690, 420, "dont open", NULL, NULL);

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
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,    // 0
			 0.5f, -0.5f, 1.0f, 0.0f,    // 1
			 0.5f,  0.5f, 1.0f, 1.0f,    // 2
			-0.5f,  0.5f, 0.0f, 1.0f     // 3
		};

		unsigned int indices[]  = {
			0, 1, 2,
			2, 3, 0
		};

		// MY_GL_CHECK(glEnable(GL_DEPTH_TEST));
		MY_GL_CHECK(glEnable(GL_BLEND));
		MY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vertex_array;
		VertexBufferLayout layout;

		VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));

		layout.push<float>(2);
		layout.push<float>(2);
		
		vertex_array.addBuffer(vertex_buffer, layout);
		
		IndexBuffer index_buffer(indices, 6);

		glm::mat4 prj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	#ifdef _WIN32
		Shader shader("../../src/Shaders/vertexShader.shader", "../../src/Shaders/fragmentShader.shader");
	#endif

	#ifdef __linux__
		Shader shader("../src/Shaders/vertexShader.shader", "../src/Shaders/fragmentShader.shader");
	#endif

		shader.bind();
		shader.setUniform4f("u_color", 0.7f, 0.0f, 0.7f, 1.0f);

	#ifdef _WIN32
		Texture texture("../../src/Textures/tester.jpg");
	#endif

	#ifdef __linux__
		Texture texture("../src/Textures/tester.jpg");
	#endif

		texture.bind();
		shader.setUniform1i("u_Texture", 0);

		vertex_array.unbind();
		vertex_buffer.unbind();
		index_buffer.unbind();
		shader.unbind();

		Renderer renderer;

		float r = 0.0f;
		float inc = 0.0f;

		/* ImGui Setup */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(gl_version);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {

			/* Render here */
			renderer.clear();

			/* ImGui New Frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.bind();
			shader.setUniform4f("u_color", r, 1.0f, 0.7f, 1.0f);

			renderer.draw(vertex_array, index_buffer, shader);

			if(r > 1.0f) { inc += -0.01f; }
			else if( r < 0.01f) { inc += 0.01f; }
			r += inc;

			ImGui::Begin("imy");
			ImGui::SliderFloat("float", &r, 0.0f, 0.0f);    // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			/* ImGui Render */
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	/* ImGui Shutdown */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
