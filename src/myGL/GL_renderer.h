#pragma once

#include "GL_error_handler.h"

#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_shader.h"

class Renderer {
	public:
		Renderer() {}
		~Renderer() {}

		/*
		 * Function - clear
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling clear it 
		 * clear buffers to preset values. 
		 * glClear sets the bitplane area of 
		 * the window to values previously 
		 * selected by glClearColor, glClearDepth 
		 * and glClearStencil.
		 */

		void clear() const {
			MY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
		}

		/*
		 * Function - clear
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling glClearColor it 
		 * specifies the red, green, blue, and alpha values 
		 * used by glClear to clear the color buffers. 
		 * Values specified by glClearColor are 
		 * clamped to the range [0,1].
		 */

		void clearColor() const {
			MY_GL_CHECK(glClearColor(0.3f, 0.0f, 0.3f, 1.0f));
		}

		/*
		 * Function - clear
		 *
		 * Parameters:
		 * [p] VertexArray &vertex_array,
		 * [p] IndexBuffer &index_buffer,
		 * [p] Shader &shader
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling draw it binds 
		 * the shaders, vertex_array and
		 * index_buffer. After binding it renders 
		 * primitives from array data.
		 */

		void draw(VertexArray &vertex_array, IndexBuffer &index_buffer, Shader &shader) const {

			shader.bind();
			// shader.setUniform4f("u_color", r, 0.0f, 0.7f, 1.0f);

			vertex_array.bind();
			index_buffer.bind();

			MY_GL_CHECK(glDrawElements(GL_TRIANGLES, index_buffer.getCount(), GL_UNSIGNED_INT, nullptr));

			// Can add unbindings but not necessary. Only for debuging
		}
};
