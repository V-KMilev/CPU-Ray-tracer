#pragma once

#include <iostream>
#include <string>

#include "GL_error_handler.h"

#include "RT_stb_image.h"

#define STBI_ONLY_PNM

class MyGLTexture {
	public:
		MyGLTexture(const std::string &file_name) : my_ID(0), my_local_buffer(nullptr), width(0), height(0), components_per_pixel(0) {
			// OGL starts its textures form the bottom so we need to flip out
			stbi_set_flip_vertically_on_load(1);

			MY_GL_CHECK(glGenTextures(1, &my_ID));
			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, my_ID));

			my_local_buffer = stbi_load(&file_name[0], &width, &height, &components_per_pixel, bytes_per_pixel);

			if (!my_local_buffer) {
				fprintf(stderr, "WARNING: Could not load texture image file \"%s\"\nSTB Reason: %s\n", &file_name[0], stbi_failure_reason());
				width  = 0;
				height = 0;
			}

			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));    // GL_TEXTURE_WRAP_S: horizontal
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));    // GL_TEXTURE_WRAP_T: vertical

			// Can with GL_RGBA and GL_RGBA8
			MY_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, my_local_buffer));

			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

			if(my_local_buffer) {
				stbi_image_free(my_local_buffer);
			}
		}

		MyGLTexture(
			unsigned int internalFormat,
			unsigned int width,
			unsigned int height,
			unsigned int format,
			unsigned int type,
			void *data
		) {
			// OGL starts its textures form the bottom so we need to flip out
			// Set to flip the data properly (vertically not horizontally)
			stbi__vertical_flip(data, width, height, sizeof(float) * 3);

			MY_GL_CHECK(glGenTextures(1, &my_ID));
			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, my_ID));

			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));    // GL_TEXTURE_WRAP_S: horizontal
			MY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));    // GL_TEXTURE_WRAP_T: vertical

			MY_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data));

			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		}

		~MyGLTexture() {
			MY_GL_CHECK(glDeleteTextures(1, &my_ID));
		}

		void update(
			unsigned int internalFormat,
			unsigned int width,
			unsigned int height,
			unsigned int format,
			unsigned int type,
			void *data
		) {
			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, my_ID));

			MY_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data));

			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		}

		// If slot not specified = 0
		void bind(unsigned int slot = 0) const {
			MY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, my_ID));
		};

		void unbind() const {
			MY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		};

	private:
		unsigned int my_ID;
		unsigned char* my_local_buffer;

		const int bytes_per_pixel = 4;
		int width, height;
		int components_per_pixel;
};
