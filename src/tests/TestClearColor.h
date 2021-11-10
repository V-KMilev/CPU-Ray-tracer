#pragma once

#include "Test.h"

namespace test {

	class TestClearColor : public Test {
		public:
			TestClearColor() : my_clear_color{ 1.0f, 0.0f, 0.0f, 1.0f} {}
			~TestClearColor() {}

			void onUpdate(float delta_time) override {}

			void onRender() override {
				MY_GL_CHECK(glClearColor(
					my_clear_color[0],
					my_clear_color[1],
					my_clear_color[2],
					my_clear_color[3]
				));
				MY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
			}

			void onImGuiRender() override {
				ImGui::ColorEdit4("ImGui: clear color", my_clear_color);
			}

		private:
			float my_clear_color[4];
	};
}
