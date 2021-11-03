// "#version 330 core" - using GLS (GL Shaders) version 330;
// "core" - blocking deprecated functions;
// "layout(location = n)" - Attribute is located at index n;
// #var - return var into char* (string);
// MY_GL_CHECK - macro;

#pragma once

#include <iostream>

#include "GL_error_handler.h"
#include "File_read.h"

#include "GL_vertex_buffer_layout.h"
#include "GL_vertex_buffer.h"
#include "GL_index_buffer.h"
#include "GL_vertex_array.h"

struct ProgramShaders {
	std::string vertex_shader;
	std::string fragment_shader;

};

static ProgramShaders getShaders() {
	ProgramShaders shaders;
	std::string vertex_shader = fileToString("../src/Shaders/vertexShader.shader");

	std::string fragment_shader = fileToString("../src/Shaders/fragmentShader.shader");

	shaders.vertex_shader = vertex_shader;
	shaders.fragment_shader = fragment_shader;\

	return shaders;
}

static unsigned int CompileShader(unsigned int type, const std::string &source) {

	unsigned int id = glCreateShader(type);
	const char* src = &source[0];    // the same as source.c_str();

	MY_GL_CHECK(glShaderSource(id, 1, &src, nullptr));
	MY_GL_CHECK(glCompileShader(id));

	/* ERROR handling */
	int result;
	MY_GL_CHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &result));    // wants int vector

	// GL_FALSE = 0
	if(result != GL_TRUE) {
		
		int length;
		MY_GL_CHECK(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));    // set length size

		char* message = (char*) alloca(length * sizeof(char));
		MY_GL_CHECK(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "[FAIL] Failed to complie " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " shader\n";
		std::cerr << message << "\n";
		
		MY_GL_CHECK(glDeleteShader(id));

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string &vertex_shader, const std::string &fragment_shader) {

	unsigned int program = glCreateProgram();    // create progam

	unsigned int vert_shad = CompileShader(GL_VERTEX_SHADER, vertex_shader);        // compile vertex shader
	unsigned int frag_shad = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);    // compile fragment shader
	

	MY_GL_CHECK(glAttachShader(program, vert_shad));    // add vertex shader
	MY_GL_CHECK(glAttachShader(program, frag_shad));    // add fragment shader

	MY_GL_CHECK(glLinkProgram(program));        // link program
	MY_GL_CHECK(glValidateProgram(program));    // validate program

	MY_GL_CHECK(glDeleteShader(vert_shad));    // delete vertex shader
	MY_GL_CHECK(glDeleteShader(frag_shad));    // delete fragment shader

	return program;
}

int window_setup() {
	
	
	GLFWwindow* window;

	glEnable(GL_DEPTH_TEST);

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Using OpenGL version 3.3.0 (major 3, minor 3)*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "dont open", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	glfwSwapInterval(1);

	float positions[] = {
		-0.5f, -0.5f,    // 0
		 0.5f, -0.5f,    // 1
		 0.5f,  0.5f,    // 2
		-0.5f,  0.5f     // 3
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};


	VertexArray vertexArray;
	VertexBufferLayout layout;
	VertexBuffer vertex_buffer(positions, 4 * 2 * sizeof(float));
	layout.push<float>(2);

	IndexBuffer index_buffer(indices, 6);


	ProgramShaders shaders = getShaders();
	unsigned int shader = CreateShader(shaders.vertex_shader, shaders.fragment_shader);
	MY_GL_CHECK(glUseProgram(shader));

	int location = glGetUniformLocation(shader, "u_color");

	MY_GL_CHECK(glBindVertexArray(0));
	MY_GL_CHECK(glUseProgram(0));
	MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	MY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.5f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MY_GL_CHECK(glUseProgram(shader));
		MY_GL_CHECK(glUniform4f(location, r, 0.0f, 0.7f, 1.0f));

		MY_GL_CHECK(glBindVertexArray(vao));
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

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
