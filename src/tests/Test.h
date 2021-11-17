#pragma once

#include "GL_error_handler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <functional>

namespace test {

	class Test {
		public:
			Test() {}
			virtual ~Test() {}

			virtual void onUpdate(float delta_time) {}

			virtual void onRender() {}

			virtual void onImGuiRender() {}
		
		private:
	};

	class TestMenu : public Test {
		public:
			TestMenu(Test* &current_test) : my_current_test(current_test){}
			~TestMenu() {}

			void onImGuiRender() override {
				for (std::pair<std::string, std::function<Test*()>> &test : my_tests) {
					if(ImGui::Button(&test.first[0])) {
						my_current_test = test.second();
					}
				}
			}

			template<typename T>
			void registerTest(const std::string test_name) {
				
				std::cerr << "[TEST] Registering test " << test_name << "\n";

				my_tests.push_back(std::make_pair(test_name, []() { return new T(); }));
			}
		
		private:
		Test* &my_current_test;
		std::vector<std::pair<std::string, std::function<Test*()>>> my_tests;
	};
}
