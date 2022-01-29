#pragma once

#include "GL_vertex_buffer_layout.h"
#include "GL_vertex_buffer.h"

class VertexArray {
	public:
		VertexArray() {
			MY_GL_CHECK(glGenVertexArrays(1, &my_ID));
		}

		~VertexArray() {
			MY_GL_CHECK(glDeleteVertexArrays(1, &my_ID));
		}

		void bind() const {
			MY_GL_CHECK(glBindVertexArray(my_ID));
		}

		void unbind() const {
			MY_GL_CHECK(glBindVertexArray(0));
		}

		void addBuffer(const VertexBuffer &vertex_buffer, const VertexBufferLayout& layout) {

			bind();
			vertex_buffer.bind();

			const std::vector<VertexBufferElement> &elements = layout.GetElements();

			unsigned int offset = 0;

			for (unsigned int position = 0; position < elements.size(); position++) {

				const VertexBufferElement &element = elements[position];

				MY_GL_CHECK(glEnableVertexAttribArray(position));
				MY_GL_CHECK(glVertexAttribPointer(position, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset));

				offset += element.count * sizeof(element.type);
			}
		}

	private:
		unsigned int my_ID;
};
