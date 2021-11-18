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

#include "Imgui_controls.h"

const char* gl_version = "#version 330";

int window_setup(std::vector<Color> pixels) {

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
		image_width, image_height,
		"dont open",
		nullptr,    // glfwGetPrimaryMonitor to set fullsrean
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
		MY_GL_CHECK(glEnable(GL_BLEND));
		MY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] = {
			0.0f               , 0.0f                , 0.0f, 0.0f,    // 0
			(float) image_width, 0.0f                , 1.0f, 0.0f,    // 1
			(float) image_width, (float) image_height, 1.0f, 1.0f,    // 2
			0.0f               , (float) image_height, 0.0f, 1.0f     // 3
		};

		unsigned int indices[]  = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray vertex_array;
		VertexBufferLayout layout;

		VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));

		layout.push<float>(2);
		layout.push<float>(2);

		vertex_array.addBuffer(vertex_buffer, layout);
		
		IndexBuffer index_buffer(indices, 6);

		#ifdef _WIN32
			Shader shader("../../src/Shaders/vertexShader.shader", "../../src/Shaders/fragmentShader.shader");
		#endif

		#ifdef __linux__
			Shader shader("../src/Shaders/vertexShader.shader", "../src/Shaders/fragmentShader.shader");
		#endif

		MyGLTexture texture(GL_RGB32F, image_width, image_height, GL_RGB, GL_FLOAT, &pixels[0]);

		texture.bind();
		shader.bind();
		shader.setUniform1i("u_Texture", 0);

		vertex_array.unbind();
		vertex_buffer.unbind();
		index_buffer.unbind();
		shader.unbind();

		Renderer renderer;

		// Sets: (left, right, bottom, top, -, -)
		glm::mat4 projection = glm::ortho(
			0.0f, (float) image_width,
			0.0f, (float) image_height,
			-1.0f, 1.0f
		);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		glm::vec3 translation(0,0,0);

		myImGui myImGui;
		myImGui.setup(window, gl_version);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {

			/* Render here */
			renderer.clear();

			MY_GL_CHECK(glClearColor(0.5f, 0.0f, 0.5f, 1.0f));

			myImGui.newFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f),translation);
				glm::mat4 mvp = projection * view * model;

				shader.bind();
				shader.setUniformMat4f("u_MVP", mvp);

				renderer.draw(vertex_array, index_buffer, shader);

				myImGui.content();
			}

			myImGui.render();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		myImGui.shutdown();
	}

	glfwTerminate();

	return 0;
}
