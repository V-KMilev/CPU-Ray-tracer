#pragma once

#include "GL_vertex_buffer.h"

#include "GL_vertex_buffer_layout.h"

class VertexArray {
	public:
		VertexArray() {
			MY_GL_CHECK(glGenVertexArrays(1, &my_ID));
		}

		~VertexArray() {
			MY_GL_CHECK(glDeleteVertexArray(1, &my_ID));
		}

		void bind() const {
			MY_GL_CHECK(glBindVertexArray(my_ID));
		}

		void unbind() const {
			MY_GL_CHECK(glBindVertexArray(0));
		}

		void AddBuffer(const VertexBuffer &vertex_buffer, const VertexBufferLayout& layout) {
			
			bind();

			vertex_buffer.bind();

			unsigned int offset = 0;

			const VectexBufferElement elements = layout.GetElements();

			for (unsigned int position = 0; position < elements.size(); position++) {

				const VectexBufferElement element = elements[position];

				MY_GL_CHECK(glEnableVertexAttribArray(position));
				MY_GL_CHECK(glVertexAttribPointer(position, element.count, element.normalized, layout.GetStride(), (const void*) offset));

				offset += element.count * sizeof(element.type);
			}

		}

	private:
		unsigned int my_ID;

};