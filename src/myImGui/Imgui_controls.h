#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Settings.h"

class myImGui {
	public:
		myImGui() {}
		~myImGui() {}

		void setup(GLFWwindow* window, const char* gl_version) {
			/* ImGui Setup */
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(gl_version);
		}

		void newFrame() {
			/* ImGui New Frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void shutdown() {
			/* ImGui Shutdown */
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void render() {
			/* ImGui Render */
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void content() {
			setStyle();

			ImGui::Begin("Menu");
			ImGui::InputFloat("Aspect ratio", &aspect_ratio);
			ImGui::InputInt("Image width", &image_width);
			ImGui::InputInt("Image height", &image_height);
			ImGui::NewLine();

			ImGui::SliderInt("Max samples per pixel", &samples_per_pixel, 0, 1337);
			ImGui::SliderInt("Max depth", &max_depth, 0, 250);
			ImGui::NewLine();
			
			ImGui::InputFloat3("Camera position", &lookfrom[0]);
			ImGui::NewLine();

			ImGui::ColorEdit3("Background color", (float*) &background);
			ImGui::NewLine();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::NewLine();

			if(ImGui::Button("Run")) {

			}
			ImGui::SameLine();

			if(ImGui::Button("Reset")) {

			}
			ImGui::SameLine();

			if(ImGui::Button("Kill")) {

			}
			ImGui::End();
		}

	private:
		void setStyle() {
			/* ImGui Style */
			ImVec4* colors = ImGui::GetStyle().Colors;

			ImGui::GetStyle().WindowRounding = 0.0f;
			ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);

			colors[ImGuiCol_TitleBgActive]           = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_TitleBg]                 = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_Button]                  = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ButtonHovered]           = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ButtonActive]            = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_SliderGrab]              = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_SliderGrabActive]        = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_FrameBg]                 = ImVec4(0.1f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.3f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_FrameBgActive]           = ImVec4(0.4f, 0.0f, 0.1f, 1.0f);

			colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.2f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.6f, 0.0f, 0.2f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.7f, 0.0f, 0.2f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_ResizeGrip]              = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.3f, 0.0f, 0.1f, 1.0f);
		}
};
