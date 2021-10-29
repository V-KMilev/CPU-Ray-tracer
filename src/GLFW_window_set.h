// "#version 330 core" - using GLS (GL Shaders) version 330;
// "core" - blocking deprecated functions;
// "layout(location = n)" - Attribute is located at index n;
// #var - return the char* var;
// MY_GL_CHECK - macro;

#pragma once

/* Future linking for glGenBuffers, glBindBuffer e.t. */
#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
#include <iostream>

/* Printing funtion fail*/
static void printGLErrors(const char* file, int line, const char* fun) {

	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("[%s:%d][ERROR %d] my_gl_checking %s\n", file, line, err, fun);
	}
}

/* do-while so we can execute MY_GL_CHECK, and we don't have to remove the ';' at the end of the function to which we my_gl_check MY_GL_CHECK*/
#define MY_GL_CHECK(MyFunction)                               \
do {                                                   \
	MyFunction;                                        \
	printGLErrors(__FILE__, __LINE__, #MyFunction);    \
} while(0)



static unsigned int CompileShader(unsigned int type, const std::string &source) {

	unsigned int id = glCreateShader(type);
	const char* src = &source[0];    // the same as source.c_str();

	MY_GL_CHECK(glShaderSource(id, 1, &src, nullptr));
	MY_GL_CHECK(glCompileShader(id));

	/* ERROR handling */
	int result;
	MY_GL_CHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &result));    // wants int vector

	if(result == GL_FALSE) {
		// GL_FALSE = 0
		int length;
		MY_GL_CHECK(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));    // set length size

		char* message = (char*) alloca(length * sizeof(char));
		MY_GL_CHECK(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "[FAIL] Failed to complie " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "shader.\n"; 
		std::cerr << message << "\n";
		
		MY_GL_CHECK(glDeleteShader(id));

		return 0;
	}	

	return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
	
	unsigned int program = glCreateProgram();    // create progam
	
	unsigned int vert_shad = CompileShader(GL_VERTEX_SHADER, vertexShader);        // compile vertex shader
	unsigned int frag_shad = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);    // compile fragment shader

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

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "dont open", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	unsigned int buffer;

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	MY_GL_CHECK(glGenBuffers(1, &buffer));
	MY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	MY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW));

	MY_GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	MY_GL_CHECK(glEnableVertexAttribArray(0));

	// TODO: CREATE VERTEX & FRAGMENT SHADER FILES

	std::string vertexShader = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main() {\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main() {\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	MY_GL_CHECK(glUseProgram(shader));

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		MY_GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

		/* Swap front and back buffers */ 
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
