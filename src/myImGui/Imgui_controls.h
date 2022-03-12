#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Settings.h"
#include "Render.h"
#include "Log.h"

class MyImGui {
	public:
		MyImGui() {}
		~MyImGui() {}

		void setup(GLFWwindow* window, const char* gl_version) {
			/* ImGui Setup */
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(gl_version);
		}

		void newframe() {
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

			ImGui::BeginMainMenuBar();

			if(ImGui::BeginMenu("File")) {
				ImGui::Checkbox("File edit", &show_file_edit);
				ImGui::Checkbox("Settings", &show_settings);
				ImGui::Checkbox("Camera", &show_camera);
				ImGui::Checkbox("Render info", &show_render_info);
				ImGui::Separator();
				ImGui::Checkbox("ImGui menu", &show_demo);
				if(ImGui::MenuItem("Exit", "Alt+F4")) {
					change_close_window = true;
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
				/* TODO: Object modifications */
			}
			ImGui::SameLine(ImGui::GetWindowWidth()-50);
			if(ImGui::MenuItem("Run")) {
				change_stop = false;
			}
			ImGui::SameLine(ImGui::GetWindowWidth()-95);
			if(ImGui::MenuItem("Stop")) {
				change_stop = true;
			}

			ImGui::SameLine(ImGui::GetWindowWidth()-147);
			if(ImGui::MenuItem("Reset")) {
				change_default = true;

				fov        = default_fov;
				max_depth  = default_max_depth;
				lookfrom   = default_lookfrom;
				lookat     = default_lookat;
				background = default_background;
			} else { change_default = false; }

			ImGui::SameLine(ImGui::GetWindowWidth()-199);
			if(ImGui::MenuItem("Clear")) {
				change_clear = true;

				max_depth  = default_max_depth;
				background = default_background;
			} else { change_clear = false; }

			ImGui::SameLine(ImGui::GetWindowWidth()/2);
			if(change_stop) { ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "STOPPED"); }
			else { ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.3f, 1.0f), "RUNNING"); }

			ImGui::EndMainMenuBar();

			if(show_demo) { ImGui::ShowDemoWindow(); }

			if(show_settings) {
				ImGui::Begin("Settings");
				ImGui::InputInt("Samples per pixel", &samples_per_pixel, 0, 1337);
				ImGui::InputInt("Max depth", &max_depth, 0, 250);
				ImGui::NewLine();

				ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel);
				if(ImGui::ColorEdit3("Background color", (float*) &background)) {
					change_bg = true;
				}
				else { change_bg = false; };
				ImGui::NewLine();

				ImGui::Checkbox("Multithreading", &change_multithreading);
				ImGui::SameLine();

				ImGui::Checkbox("Static render", &change_static);
				ImGui::NewLine();

				ImGui::Separator();
				ImGui::End();
			}

			if(show_camera) {
				ImGui::Begin("Camera");

				if(ImGui::SliderFloat3("Camera position", &lookfrom[0], -13.0f, 13.0f)) {
					change_position = true;
				}
				else { change_position = false; }

				if(ImGui::SliderFloat3("Camera focus", &lookat[0], -13.0f, 13.0f)) {
					change_view = true;
				}
				else { change_view = false; }

				if(ImGui::InputFloat("Camera aperture ", &aperture, 0.0f, 10.0f)) {
					change_fov = true;
				}
				else { change_fov = false; }

				if(ImGui::InputFloat("Camera fov ", &fov, 0.0f, 180.0f)) {
					change_aperture = true;
				}
				else { change_aperture = false; }
				ImGui::NewLine();

				ImGui::InputFloat("Move precision", &precision, 0, 37);
				ImGui::NewLine();

				ImGui::Separator();
				ImGui::End();
			}

			if(show_render_info) {
				ImGui::Begin("INFO");

				ImGui::Separator();
				ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "BASIC INFO");
				ImGui::NewLine();

				ImGui::Text("Total buckets: %d", total_buckets);

				ImGui::Text("Bucket size: %d", bucket_size);

				ImGui::Text("Max Threads: %d", MAX_NUMBER_OF_THREADS);
				ImGui::NewLine();

				static float values[120] = {};
				static int values_offset = 0;
				static double refresh_time = 0.0;

				// Create data at fixed 60 Hz rate for the demo
				while (refresh_time < ImGui::GetTime()) {
					values[values_offset] = ImGui::GetIO().Framerate;
					values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
					refresh_time += 1.0f / 60.0f;
				}

				// Plots can display overlay texts
				// (in this example, we will display an average value)
				{
					float average = 0.0f;
					for (int n = 0; n < IM_ARRAYSIZE(values); n++)
						average += values[n];
					average /= (float)IM_ARRAYSIZE(values);
					char overlay[32];
					sprintf(overlay, "avg %.3f", average);
					ImGui::PlotLines("FPS", values, IM_ARRAYSIZE(values), values_offset, overlay);
				}

				ImGui::Separator();
				ImGui::End();
			}

			static int corner = 0;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

			if (corner != -1) {
				const float PAD = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();

				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;

				window_pos.x = (3 & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (3 & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (3 & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (3 & 2) ? 1.0f : 0.0f;

				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			}
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("Info overlay", &show_overlay, window_flags)) {
				ImGui::Text("Benchmark - Ray-tracer v1.00");
				ImGui::Separator();
				ImGui::Text("%.3f ms/frame | %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				
				ImGui::End();
			}

			if(show_file_edit) {
				ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("File edit")) {

					static int selected = 0;
					static std::vector<std::shared_ptr<Hittable>> &my_objects = world.objects;
					{
						ImGui::BeginChild("left pane", ImVec2(150, 0), true);
						for (int i = 0; i < my_objects.size(); i++){

							char label[128];

							sprintf(label, "%d | %s", i, my_objects[i]->object_name);
							if (ImGui::Selectable(label, selected == i)) { selected = i; }
						}
						ImGui::EndChild();
					}
					ImGui::SameLine();

					{
						ImGui::BeginGroup();
						ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
						ImGui::Text("%d | %s", selected,  my_objects[selected]->object_name);
						
						if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
							if (ImGui::BeginTabItem("Edit")) {
								ImGui::TextWrapped("Controls:");

								if(my_objects[selected]->id == 1) {
									std::shared_ptr<xy_rect> object = std::dynamic_pointer_cast<xy_rect>(my_objects[selected]);
									
									ImGui::InputFloat("Start X:", &object->x_start);
									ImGui::InputFloat("Start Y:", &object->y_start);
									ImGui::InputFloat("End X:", &object->x_end);
									ImGui::InputFloat("End Y:", &object->y_end);
									ImGui::InputFloat("z:", &object->z);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}

								if(my_objects[selected]->id == 2) {
									std::shared_ptr<xz_rect> object = std::dynamic_pointer_cast<xz_rect>(my_objects[selected]);

									ImGui::InputFloat("Start X:", &object->x_start);
									ImGui::InputFloat("Start Z:", &object->z_start);
									ImGui::InputFloat("End X:", &object->x_end);
									ImGui::InputFloat("End Z:", &object->z_end);
									ImGui::InputFloat("Y:", &object->y);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}

								if(my_objects[selected]->id == 3) {
									std::shared_ptr<yz_rect> object = std::dynamic_pointer_cast<yz_rect>(my_objects[selected]);

									ImGui::InputFloat("Start Y:", &object->y_start);
									ImGui::InputFloat("Start Z:", &object->z_start);
									ImGui::InputFloat("End Y:", &object->y_end);
									ImGui::InputFloat("End Z:", &object->z_end);
									ImGui::InputFloat("X:", &object->x);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}

								if (my_objects[selected]->id == 4) {
									std::shared_ptr<obj> object = std::dynamic_pointer_cast<obj>(my_objects[selected]);

									// ImGui::InputFloat3("Position:", &object->postion);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}

								if (my_objects[selected]->id == 5) {
									std::shared_ptr<Sphere_moving> object = std::dynamic_pointer_cast<Sphere_moving>(my_objects[selected]);

									ImGui::InputFloat3("Position C0:", &object->center_0[0]);
									ImGui::InputFloat3("Position C1:", &object->center_1[0]);
									ImGui::InputFloat("Radius:", &object->radius);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}

								if (my_objects[selected]->id == 6) {
									std::shared_ptr<Sphere> object = std::dynamic_pointer_cast<Sphere>(my_objects[selected]);

									ImGui::InputFloat3("Position:", &object->center[0]);
									ImGui::InputFloat("Radius:", &object->radius);

									ImGui::NewLine();
									// ImGui::Text("Material: %s", object->material_ptr->my_name);
									// ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									// object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
								}
								ImGui::Separator();
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Details")) {
								if(my_objects[selected]->id == 1) {
									std::shared_ptr<xy_rect> object = std::dynamic_pointer_cast<xy_rect>(my_objects[selected]);

									ImGui::Text("Position:\nx-start: %.3f\ny-start: %.3f\nx-end: %.3f\ny-end: %.3f\nz: %.3f",
									object->x_start, object->y_start, object->x_end, object->y_end, object->z);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}

								if(my_objects[selected]->id == 2) {
									std::shared_ptr<xz_rect> object = std::dynamic_pointer_cast<xz_rect>(my_objects[selected]);

									ImGui::Text("Position:\nx-start: %.3f\nz-start: %.3f\nx-end: %.3f\nz-end: %.3f\ny: %.3f",
									object->x_start, object->z_start, object->x_end, object->z_end, object->y);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}

								if(my_objects[selected]->id == 3) {
									std::shared_ptr<yz_rect> object = std::dynamic_pointer_cast<yz_rect>(my_objects[selected]);

									ImGui::Text("Position:\ny-start: %.3f\nz-start: %.3f\ny-end: %.3f\nz-end: %.3f\nx: %.3f",
									object->y_start, object->z_start, object->y_end, object->z_end, object->x);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}

								if (my_objects[selected]->id == 4) {
									std::shared_ptr<obj> object = std::dynamic_pointer_cast<obj>(my_objects[selected]);

									// ImGui::Text("Position:\nx: %.3f y: %.3f z: %.3f",
									// object->, my_objects[selected]->position[2]);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}

								if (my_objects[selected]->id == 5) {
									std::shared_ptr<Sphere_moving> object = std::dynamic_pointer_cast<Sphere_moving>(my_objects[selected]);

									ImGui::Text("C0 Position:\nx: %.3f y: %.3f z: %.3f",
									object->center_0[0], object->center_0[1], object->center_0[2]);
									ImGui::Text("C1 Position:\nx: %.3f y: %.3f z: %.3f",
									object->center_1[0], object->center_1[1], object->center_1[2]);
									ImGui::Text("Radius: %.3f", object->radius);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}

								if (my_objects[selected]->id == 6) {
									std::shared_ptr<Sphere> object = std::dynamic_pointer_cast<Sphere>(my_objects[selected]);

									ImGui::Text("Position:\nx: %.3f y: %.3f z: %.3f",
									object->center[0], object->center[1], object->center[2]);
									ImGui::Text("Radius: %.3f", object->radius);

									ImGui::NewLine();
									ImGui::Text("Material: %s", object->material_ptr->my_name);
									ImGui::Text("Color: r: %.3f g: %.3f b: %.3f",
									object->material_ptr->my_color[0], object->material_ptr->my_color[1], object->material_ptr->my_color[2]);
									ImGui::NewLine();
									ImGui::Text("ID: %d", object->id);
								}
								ImGui::Separator();
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
						}
						ImGui::EndChild();


						if (ImGui::Button("ADD")) {
							ImGui::OpenPopup("my_select_popup");
						}
						const char* objects_names[] = { "xy_rect", "xz_rect", "yz_rect", "obj", "sphere_moving", "sphere" };

						if (ImGui::BeginPopup("my_select_popup")) {
							ImGui::Text("Objects");
							ImGui::Separator();

							for (int i = 0; i < IM_ARRAYSIZE(objects_names); i++) {
								if (ImGui::Selectable(objects_names[i])) {
								}
							}
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						if (ImGui::Button("REMOVE")) {
							world.remove(selected);
						}
						ImGui::EndGroup();
					}
				}
				ImGui::End();
			}

			// ImGui::Begin("Console log");

			// const std::string &msg = Logger::getDefaultLogger().str();

			// ImGui::InputTextMultiline("", (char*)msg.c_str(), msg.length());
			// ImGui::End();
		}

	private:
		void setStyle() {
			/* ImGui Style */
			ImGuiStyle& style = ImGui::GetStyle();

			style.FrameBorderSize = 1.0f;
			style.WindowBorderSize = 0.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;

			ImGui::GetStyle().WindowRounding = 0.0f;
			ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

			colors[ImGuiCol_TitleBgActive]        = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_TitleBg]              = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_Button]               = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ButtonHovered]        = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ButtonActive]         = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_SliderGrab]           = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_SliderGrabActive]     = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_FrameBg]              = ImVec4(0.1f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.3f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_FrameBgActive]        = ImVec4(0.4f, 0.0f, 0.1f, 1.0f);

			colors[ImGuiCol_CheckMark]            = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_Separator]            = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.2f, 0.0f, 0.1f, 1.0f);
			colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.6f, 0.0f, 0.2f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.7f, 0.0f, 0.2f, 1.0f);
			colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_ResizeGrip]           = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_Header]               = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_HeaderHovered]        = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_HeaderActive]         = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_Tab]                  = ImVec4(0.7f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_TabHovered]           = ImVec4(0.6f, 0.0f, 0.3f, 1.0f);
			colors[ImGuiCol_TabActive]            = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

			colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.3f, 0.0f, 0.1f, 1.0f);
		}
	private:
		bool show_settings    = false;
		bool show_camera      = false;
		bool show_render_info = false;
		bool show_file_edit   = false;
		bool show_demo        = false;
		bool show_overlay     = true;
};
