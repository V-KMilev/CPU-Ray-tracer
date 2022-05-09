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
#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_renderer.h"
#include "GL_texture.h"
#include "GL_shader.h"

#include "Imgui_controls.h"
#include "Glfw_controls.h"

#include "Thread_manager.h"
#include "File_write.h"
#include "Render.h"

const char* gl_version = "#version 330";

// Sets: (left, right, bottom, top, -, -)
glm::mat4 projection = glm::ortho(
	0.0f, (float) image_width,
	0.0f, (float) image_height,
	-1.0f, 1.0f
);
// Set triangle positions scale
glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
glm::mat4 mvp = projection * view * model;

int frame_number = 0;

int window_setup() {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Using OpenGL version 3.3.0 (major 3, minor 3) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(
		image_width, image_height,
		"dont open",
		nullptr,    // glfwGetPrimaryMonitor to set fullscrean
		nullptr
	);

	if (!window) { glfwTerminate(); return -1; }

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	// We have this scope so we don't have to make everything pointers or new and then delete
	{
		MY_GL_CHECK(glEnable(GL_BLEND));
		MY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float positions[] = {
			0.0f               , 0.0f                , 0.0f, 1.0f,    // 0
			(float) image_width, 0.0f                , 1.0f, 1.0f,    // 1
			(float) image_width, (float) image_height, 1.0f, 0.0f,    // 2
			0.0f               , (float) image_height, 0.0f, 0.0f     // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray vertex_array;
		VertexBufferLayout layout;

		VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));

		layout.push<float>(2);
		layout.push<float>(2);

		vertex_array.addBuffer(vertex_buffer, layout);

		IndexBuffer index_buffer(indices, 6);

		#ifdef _WIN32
			Shader shader("../../src/shaders/vertexShader.shader", "../../src/shaders/fragmentShader.shader");
		#endif

		#ifdef __linux__
			Shader shader("../src/shaders/vertexShader.shader", "../src/shaders/fragmentShader.shader");
		#endif

		MyGLTexture texture(GL_RGB32F, image_width, image_height, GL_RGB, GL_FLOAT, &pixels[0]);
		texture.bind();

		shader.bind();
		shader.setUniform1i("u_Texture", 0);
		shader.setUniformMat4f("u_MVP", mvp);

		vertex_array.unbind();
		vertex_buffer.unbind();
		index_buffer.unbind();
		shader.unbind();

		Renderer renderer;

		MyImGui myImGui;
		myImGui.setup(window, gl_version);

		MyGlfw myGlfw(window);

		ThreadPool pool_multithread(MAX_NUMBER_OF_THREADS);
		ThreadPool pool_onethread(1);

		std::vector<Bucket> my_buckets = bucket_segmentation(image_width, image_height);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* OpenGL RENDER: */
			renderer.clear();
			renderer.clearColor();

			myGlfw.fullControlSet(precision);

			myImGui.newframe();

			if(change_multithreading) {
				pool_onethread.clear();

				for (Bucket &my_bucket : my_buckets) {
					pool_multithread.enter_queue(my_bucket);
				}
			} else {
				pool_multithread.clear();

				for (Bucket &my_bucket : my_buckets) {
					pool_onethread.enter_queue(my_bucket);
				}
			}

			if(change_camera || change_scene || change_object || change_object_list ||
				change_clear || change_default) {
				if(change_multithreading) {
					pool_multithread.clear();
				}
				else {
					pool_onethread.clear();
				}

				/* Reset Render samples counter */
				samples_in_counter.store(0);

				/* Reset bucket counter counter */
				buckets_in_counter.store(0);

				/* Reset scene counter */
				scenes_in_counter.store(0);

				samples_per_pixel = default_samples_per_pixel;

				pixels            = empty_pixels;
				samples_in_pixels = empty_samples_in_pixels;

				change_object_list   = false;
				change_object        = false;
				change_camera        = false;
				change_scene         = false;
				change_clear         = false;
				change_default       = false;
				change_edit_stop     = false;

				frame_number = 0;
			}

			#ifdef DEBUG
			{
				char fileName[256] = {0};

				#ifdef _WIN32
					sprintf(fileName, "../../src/textures/debug/ppm_img/RTout%d.ppm", frame_number);
				#endif

				#ifdef __linux__
					sprintf(fileName, "../src/textures/debug/ppm_img/RTout%d.ppm", frame_number);
				#endif

				if(samples_per_pixel % 3 == 0) {
					std::ofstream out(fileName);

					file_write(out, pixels, image_width, image_height);

					out.close();

					frame_number++;
				}
			}
			#endif

			{
				texture.update(GL_RGB32F, image_width, image_height, GL_RGB, GL_FLOAT, &pixels[0]);
				texture.bind();

				shader.bind();
				shader.setUniform1i("u_Texture", 0);

				renderer.draw(vertex_array, index_buffer, shader);

				myImGui.content();
			}

			myImGui.render();

			if(change_close_window) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		myImGui.shutdown();
	}

	embree.release();

	glfwTerminate();

	return 0;
}
