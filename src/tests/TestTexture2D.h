// glm::ortho set: (left, right, bottom, top, -, -)

#pragma once

#include "Test.h"

#include "GL_error_handler.h"
#include "GL_shader.h"
#include "GL_texture.h"
#include "GL_renderer.h"
#include "GL_vertex_array.h"
#include "GL_index_buffer.h"
#include "GL_vertex_buffer.h"
#include "GL_vertex_buffer_layout.h"

#include "Utility_functions.h"

namespace test {

	class TestTexture2D : public Test {
		public:
			TestTexture2D() :
			my_projection(glm::ortho(0.0f, 1337.0f, 0.0f, 1337.0f, -1.0f, 1.0f)),
			my_view(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
			my_translation0(0,0,0), my_translation1(0,0,0) {

				float positions[] = {
					-200.0f, -150.0f, 0.0f, 0.0f,    // 0
					 200.0f, -150.0f, 1.0f, 0.0f,    // 1
					 200.0f, 150.0f, 1.0f, 1.0f,    // 2
					-200.0f, 150.0f, 0.0f, 1.0f     // 3
				};

				unsigned int indices[]  = {
					0, 1, 2,
					2, 3, 0
				};

				my_vertex_array = std::make_unique<VertexArray>();

				my_vertex_buffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

				VertexBufferLayout layout;

				layout.push<float>(2);
				layout.push<float>(2);

				my_vertex_array->addBuffer(*my_vertex_buffer, layout);

				my_index_buffer = std::make_unique<IndexBuffer>(indices, 6);

				#ifdef _WIN32
					my_shader = std::make_unique<Shader>("../../src/Shaders/vertexShader.shader", "../../src/Shaders/fragmentShader.shader");
					my_texture = std::make_unique<MyGLTexture>("../../src/Textures/tester.jpg");
				#endif

				#ifdef __linux__
					my_shader = std::make_unique<Shader>("../src/Shaders/vertexShader.shader", "../src/Shaders/fragmentShader.shader");
					my_texture = std::make_unique<MyGLTexture>("../src/Textures/tester.jpg");
				#endif

				my_shader->setUniform1i("u_Texture", 0);
			}

			~TestTexture2D() {}

			void onUpdate(float delta_time) override {}

			void onRender() override {

				MY_GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
				MY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

				Renderer renderer;

				my_texture->bind();

				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), my_translation0);
					glm::mat4 mvp = my_projection * my_view * model;

					my_shader->bind();
					my_shader->setUniformMat4f("u_MVP", mvp);

					renderer.draw(*my_vertex_array, *my_index_buffer, *my_shader);
				}
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), my_translation1);
					glm::mat4 mvp = my_projection * my_view * model;

					my_shader->bind();
					my_shader->setUniformMat4f("u_MVP", mvp);

					renderer.draw(*my_vertex_array, *my_index_buffer, *my_shader);
				}
			}

			void onImGuiRender() override {
				ImGui::SliderFloat3("Translation 0:", &my_translation0.x, 0.0f, 1337.0f);    // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat3("Translation 1:", &my_translation1.x, 0.0f, 1337.0f);    // Edit 1 float using a slider from 0.0f to 1.0f

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

		private:
			std::unique_ptr<VertexArray> my_vertex_array;
			std::unique_ptr<VertexBuffer> my_vertex_buffer;
			std::unique_ptr<IndexBuffer> my_index_buffer;
			std::unique_ptr<Shader> my_shader;
			std::unique_ptr<MyGLTexture> my_texture;

			glm::vec3 my_translation0;
			glm::vec3 my_translation1;

			glm::mat4 my_projection;
			glm::mat4 my_view;
	};
}
