#pragma once

#include "GL_error_handler.h"

#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_shader.h"

class Renderer {
	public:
		Renderer() {}
		
		~Renderer() {}

		void clear() const {
			MY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
		}

		void clearColor() const {
			MY_GL_CHECK(glClearColor(0.3f, 0.0f, 0.3f, 1.0f));
		}

		void draw(VertexArray &vertex_array, IndexBuffer &index_buffer, Shader &shader) const {

			shader.bind();
			// shader.setUniform4f("u_color", r, 0.0f, 0.7f, 1.0f);

			vertex_array.bind();
			index_buffer.bind();

			MY_GL_CHECK(glDrawElements(GL_TRIANGLES, index_buffer.getCount(), GL_UNSIGNED_INT, nullptr));

			// Can add unbindings but not necessary. Only for debuging
		}
};
