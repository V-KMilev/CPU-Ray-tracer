#pragma once

#include <unordered_map>
#include <string>

#include "GL_error_handler.h"
#include "File_read.h"


struct ProgramShaders {

	/*
	 * struct - Constructor
	 *
	 * Parameters:
	 * [p] const std::string &vertex_shader,
	 * [p] const std::string &fragment_shader
	 * 
	 * Use:
	 * Sets vertex_shader and fragment_shader.
	 */

	ProgramShaders(const std::string &vertex_shader, const std::string &fragment_shader) 
	: vertex_shader(vertex_shader), fragment_shader(fragment_shader) {}

	const std::string vertex_shader;
	const std::string fragment_shader;
};

class Shader {
	public:
		Shader(const std::string &vertex_file_name, const std::string &fragment_file_name) 
		: my_vertex_file_name(vertex_file_name), my_fragment_file_name(fragment_file_name), my_ID(0) {

			ProgramShaders source = getShaders(vertex_file_name, fragment_file_name);
			my_ID = createShader(source.vertex_shader, source.fragment_shader);
		}

		~Shader() {
			MY_GL_CHECK(glDeleteProgram(my_ID));
		}

		/*
		 * Function - bind
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling bind it 
		 * installs a program object 
		 * as part of current rendering.
		 */

		void bind() const {
			MY_GL_CHECK(glUseProgram(my_ID));
		}

		/*
		 * Function - unbind
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling unbind we 
		 * install the 0 object = none 
		 * (free mem.).
		 */

		void unbind() const {
			MY_GL_CHECK(glUseProgram(0));
		}

		/*
		 * Function - setUniform4f
		 *
		 * Parameters:
		 * [p] const std::string &uniform_name,
		 * [p] float f0,
		 * [p] float f1,
		 * [p] float f2,
		 * [p] float f3
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling setUniform4f we 
		 * specify the value of a uniform 
		 * variable for the current program object.
		 * Specifying 4 floats.
		 */

		// Set uniforms
		void setUniform4f(const std::string &uniform_name, float f0, float f1, float f2, float f3) {

			int location = getUniformLocation(uniform_name);

			MY_GL_CHECK(glUniform4f(location, f0, f1, f2, f3));
		}

		/*
		 * Function - setUniform1i
		 *
		 * Parameters:
		 * [p] const std::string &uniform_name,
		 * [p] int i0
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling setUniform1i we 
		 * specify the value of a uniform 
		 * variable for the current program object.
		 * Specifying int.
		 */

		void setUniform1i(const std::string &uniform_name, int i0) {

			int location = getUniformLocation(uniform_name);

			MY_GL_CHECK(glUniform1i(location, i0));
		}

		/*
		 * Function - setUniformMat4f
		 *
		 * Parameters:
		 * [p] const std::string &uniform_name,
		 * [p] const glm::mat4 &matrix
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling setUniformMat4f we 
		 * specify the value of a uniform 
		 * variable for the current program object. 
		 * Specifying float matrix 4x4.
		 */

		void setUniformMat4f(const std::string &uniform_name, const glm::mat4 &matrix) {

			int location = getUniformLocation(uniform_name);

			MY_GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
		}

	private:

		/*
		 * Function - getUniformLocation
		 *
		 * Parameters:
		 * [p] const std::string &name
		 * 
		 * Return type:
		 * int
		 * 
		 * Use:
		 * By calling getUniformLocation we 
		 * get in return the location of the 
		 * uniform we specified.
		 */

		int getUniformLocation(const std::string &name) {
			// If location already exist we just reuse it
			if(my_uniform_location_cache.find(name) != my_uniform_location_cache.end()) {
				return my_uniform_location_cache[name];
			}

			int location = glGetUniformLocation(my_ID, &name[0]);

			// -1 means we dont have such unifrom
			if(location == -1) { std::cerr << "WARNING: uniform '" << name << "' deosn't exist\n"; }

			my_uniform_location_cache[name] = location;
			return location;
		}

		/*
		 * Function - getShaders
		 *
		 * Parameters:
		 * [p] const std::string &vertex_file_name,
		 * [p] const std::string &fragment_file_name
		 * 
		 * Return type:
		 * ProgramShaders
		 * 
		 * Use:
		 * By calling getUniformLocation we 
		 * get in return the shaders. 
		 * We read the vertex and fragment
		 * files and import them in string format.
		 */

		ProgramShaders getShaders(const std::string &vertex_file_name, const std::string &fragment_file_name) {

			const std::string vertex_shader = fileToString(vertex_file_name);
			const std::string fragment_shader = fileToString(fragment_file_name);

			ProgramShaders shaders(vertex_shader, fragment_shader);

			return shaders;
		}

		/*
		 * Function - CompileShader
		 *
		 * Parameters:
		 * [p] unsigned int type,
		 * [p] const std::string &source
		 * 
		 * Return type:
		 * unsigned int
		 * 
		 * Use:
		 * By calling CompileShader we 
		 * compile the imported shaders and
		 * get in return the shaders id.
		 */

		unsigned int CompileShader(unsigned int type, const std::string &source) {

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

				id = 0;
			}

			return id;
		}

		/*
		 * Function - createShader
		 *
		 * Parameters:
		 * [p] const std::string &vertex_shader, 
		 * [p] const std::string &fragment_shader
		 * 
		 * Return type:
		 * unsigned int
		 * 
		 * Use:
		 * By calling CompileShader we 
		 * create program and attach the shaders
		 * to it. We get in return the program.
		 */

		unsigned int createShader(const std::string &vertex_shader, const std::string &fragment_shader) {

			unsigned int program = glCreateProgram();    // create progam

			unsigned int vertex_shad = CompileShader(GL_VERTEX_SHADER, vertex_shader);        // compile vertex shader
			unsigned int fragment_shad = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);    // compile fragment shader

			MY_GL_CHECK(glAttachShader(program, vertex_shad));    // add vertex shader
			MY_GL_CHECK(glAttachShader(program, fragment_shad));    // add fragment shader

			MY_GL_CHECK(glLinkProgram(program));        // link program
			MY_GL_CHECK(glValidateProgram(program));    // validate program

			MY_GL_CHECK(glDeleteShader(vertex_shad));    // delete vertex shader
			MY_GL_CHECK(glDeleteShader(fragment_shad));    // delete fragment shader

			return program;
		}

	private:
		unsigned int my_ID;
		std::unordered_map<std::string, int> my_uniform_location_cache;

		/* DONT USE THEM, debug only */
		const std::string my_vertex_file_name;
		const std::string my_fragment_file_name;
};
