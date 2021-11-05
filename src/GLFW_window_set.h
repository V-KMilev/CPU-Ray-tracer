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

#include "GL_vertex_buffer_layout.h"
#include "GL_vertex_buffer.h"
#include "GL_index_buffer.h"
#include "GL_vertex_array.h"
#include "GL_shader.h"

int window_setup() {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Using OpenGL version 3.3.0 (major 3, minor 3) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "dont open", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	float positions[] = {
		-0.5f, -0.5f,    // 0
		 0.5f, -0.5f,    // 1
		 0.5f,  0.5f,    // 2
		-0.5f,  0.5f     // 3
	};

	unsigned int indices[]  = {
		0, 1, 2,
		2, 3, 0
	};

	MY_GL_CHECK(glEnable(GL_DEPTH_TEST));

	VertexArray vertex_array;
	VertexBufferLayout layout;

	VertexBuffer vertex_buffer(positions, 2 * 4 * sizeof(float));

	layout.push<float>(2);
	
	vertex_array.addBuffer(vertex_buffer, layout);
	
	IndexBuffer index_buffer(indices, 6);

	Shader shader("../src/Shaders/vertexShader.shader", "../src/Shaders/fragmentShader.shader");
	shader.bind();
	shader.setUniform4f("u_color", 0.7f, 0.0f, 0.7f, 1.0f);

	vertex_array.unbind();
	vertex_buffer.unbind();
	index_buffer.unbind();
	shader.unbind();

	float r = 0.0f;
	float increment = 0.5f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.bind();
		shader.setUniform4f("u_color", r, 0.0f, 0.7f, 1.0f);

		vertex_array.bind();
		index_buffer.bind();

		MY_GL_CHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if(r > 1.0f) {
			increment = -0.05f;
		} else if( r < 0.0f) {
			increment = 0.05f;
		}

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
