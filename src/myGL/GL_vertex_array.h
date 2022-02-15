#pragma once

#include "GL_vertex_buffer_layout.h"
#include "GL_vertex_buffer.h"

class VertexArray {
	public:

		/*
		 * Class - Constructor
		 *
		 * Parameters:
		 * none
		 * 
		 * Use:
		 * Genereates a vertex-array.
		 */

		VertexArray() {
			MY_GL_CHECK(glGenVertexArrays(1, &my_ID));
		}

		/*
		 * Class - Deconstructor
		 *
		 * Parameters:
		 * none
		 * 
		 * Use:
		 * By de-creating vertex-arrays we 
		 * delete the last binded vertex.
		 */

		~VertexArray() {
			MY_GL_CHECK(glDeleteVertexArrays(1, &my_ID));
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
		 * By calling bind we bind 
		 * the vertexArray with the corresponding id.
		 */

		void bind() const {
			MY_GL_CHECK(glBindVertexArray(my_ID));
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
		 * By calling unbind we bind 
		 * the 0 vertex-array = none (free mem.).
		 */

		void unbind() const {
			MY_GL_CHECK(glBindVertexArray(0));
		}

		/*
		 * Function - addBuffer
		 *
		 * Parameters:
		 * [p] const VertexBuffer &vertex_buffer,
		 * [p] const VertexBufferLayout& layout
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling addBuffer we bind vertex-array, 
		 * vertex-buffer and get the layout for the vertex-buffer. 
		 * glVertexAttribPointer and glVertexAttribIPointer 
		 * specify the location and data format of the 
		 * array of generic vertex attributes at index 
		 * index to use when rendering.
		 */

		void addBuffer(const VertexBuffer &vertex_buffer, const VertexBufferLayout& layout) {

			bind();
			vertex_buffer.bind();

			const std::vector<VertexBufferElement> &elements = layout.getElements();

			unsigned int offset = 0;

			for (unsigned int position = 0; position < elements.size(); position++) {

				const VertexBufferElement &element = elements[position];

				MY_GL_CHECK(glEnableVertexAttribArray(position));
				MY_GL_CHECK(glVertexAttribPointer(position, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset));

				/* The offset equals to the count of the previous element times the size of the element type */
				offset += element.count * sizeof(element.type);
			}
		}

	private:
		unsigned int my_ID;
};
