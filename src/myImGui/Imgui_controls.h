#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Settings.h"
#include "Render.h"
#include "Log.h"

enum Resolution_ID : unsigned int {
	t_default   = 1,
	t_test      = 2,
	t_hd        = 3,
	t_full_hd   = 4,
	t_2k        = 5,
	t_4k        = 6
};

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
				ImGui::Separator();
				ImGui::MenuItem("ImGui menu", "Ctrl+D", &show_demo);
				if(ImGui::MenuItem("Exit", "Alt+F4")) {
					show_exit = true;
				} else { show_exit = false; }
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Edit")) {
				ImGui::MenuItem("File edit", NULL, &show_file_edit);
				ImGui::MenuItem("Settings", NULL, &show_render);
				ImGui::MenuItem("Camera", NULL, &show_camera);
				ImGui::MenuItem("Details", NULL, &show_details);
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Debug")) {
				ImGui::MenuItem("Debug", NULL, &show_debug);
				ImGui::EndMenu();
			}

			const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

			static float clear_width = 100.0f; //The 100.0f is just a guess size for the first frame.
			float pos = clear_width + ItemSpacing;
			ImGui::SameLine(ImGui::GetWindowWidth() - pos);
			if(ImGui::MenuItem("Clear")) {
				change_clear = true;

				clear_scene();
			}
			clear_width = ImGui::GetItemRectSize().x; //Get the actual width for next frame.

			static float reset_width = 100.0f;
			pos += reset_width + ItemSpacing;
			ImGui::SameLine(ImGui::GetWindowWidth() - pos);
			if(ImGui::MenuItem("Reset")) {
				change_default = true;

				reset_scene();
			}
			reset_width = ImGui::GetItemRectSize().x; //Get the actual width for next frame.

			static float stop_width = 100.0f;
			pos += stop_width + ItemSpacing;
			ImGui::SameLine(ImGui::GetWindowWidth() - pos);
			if(ImGui::MenuItem("Stop")) {
				change_force_stop = true;
			}
			stop_width = ImGui::GetItemRectSize().x; //Get the actual width for next frame.

			static float run_width = 100.0f;
			pos += run_width + ItemSpacing;
			ImGui::SameLine(ImGui::GetWindowWidth() - pos);
			if(ImGui::MenuItem("Run")) {
				change_force_stop = false;
			}
			run_width = ImGui::GetItemRectSize().x; //Get the actual width for next frame.

			ImGui::SameLine(ImGui::GetWindowWidth()/2);
			if(change_force_stop || change_edit_stop) { ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "STOPPED"); }
			else { ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.3f, 1.0f), "RUNNING"); }

			ImGui::EndMainMenuBar();

			settings();
			camera_control();
			file_edit();
			render_info();

			exit();

			ImGuiWindowFlags my_window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;

			const float PAD = 100.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();

			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;

			ImVec2 debug_window_pos, debug_window_pos_pivot;

			debug_window_pos.x = work_size.x - 210;
			debug_window_pos.y = work_pos.y;

			ImGui::SetNextWindowPos(debug_window_pos, ImGuiCond_Always, debug_window_pos_pivot);
			ImGui::SetNextWindowBgAlpha(0.47f); // Transparent background
			debug(my_window_flags);

			if(show_demo) { ImGui::ShowDemoWindow(); }

			right_low_corener_info();
		}

	private:
		void exit() {
			if(show_exit)
				ImGui::OpenPopup("Exit?");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Are you sure you want to exit?\nAll files will be deleted!\n\n");
				ImGui::Separator();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PopStyleVar();

				if (ImGui::Button("OK", ImVec2(100, 0))) { change_close_window = true; }
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(100, 0))) {
					show_exit = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		void right_low_corener_info() {
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
		}

		void settings() {
			if(show_render) {
				ImGui::Begin("Settings", &show_render);
					ImGui::Separator();
					ImGui::InputInt("Samples per pixel", &samples_per_pixel, 0, 1337);
					ImGui::InputInt("Max depth", &max_depth, 0, 250);
					ImGui::NewLine();

					if(ImGui::ColorEdit3("Background color", (float*) &background)) {
						change_scene = true;
						change_edit_stop = true;
					}
					ImGui::NewLine();

					ImGui::Checkbox("Multithreading", &change_multithreading);
					ImGui::SameLine();

					ImGui::Checkbox("Static render", &change_static);
					ImGui::SameLine();

					if(ImGui::Checkbox("Single cast", &change_single_cast)) {
						change_scene = true;
						change_edit_stop = true;
					}
					ImGui::NewLine();

					ImGui::Separator();
				ImGui::End();
			}
		}

		void camera_control() {
			if(show_camera) {
				ImGui::Begin("Camera", &show_camera);
					ImGui::Separator();
					if(ImGui::InputFloat3("Camera position", &lookfrom[0])) {
						change_camera = true;
						change_edit_stop = true;
					}
					if(ImGui::InputFloat3("Camera focus", &lookat[0])) { 
						change_camera = true;
						change_edit_stop = true;
					}
					ImGui::NewLine();

					if(ImGui::InputFloat("Camera aperture ", &aperture, 0.0f, 10.0f)) {
						change_camera = true;
						change_edit_stop = true;
					}

					if(ImGui::InputFloat("Camera fov ", &fov, 0.0f, 180.0f)) {
						change_camera = true;
						change_edit_stop = true;
					}
					ImGui::NewLine();

					ImGui::InputFloat("Move precision", &precision, 0, 37);
					ImGui::NewLine();

					ImGui::Separator();
				ImGui::End();
			}
		}

		void render_info() {
			if(show_details) {
				ImGui::Begin("Details", &show_details);
					ImGui::Separator();
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
		}

		template<class T>
		void object_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, T* object) {

			object_id = object->id;

			ImGui::NewLine();

			if(object->material_ptr->id == t_lambertian) {
				Lambertian* material = static_cast<Lambertian*>(object->material_ptr.get());

				material_id = material->id;

				ImGui::Text("Material: %s", material->my_name);

				if(material->albedo->id == t_solid_color) {
					Solid_Color* texture = static_cast<Solid_Color*>(material->albedo.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture color:\nr: %.3f | g: %.3f | b: %.3f",
					texture->color_value[0], texture->color_value[1], texture->color_value[2]);
				}
				if(material->albedo->id == t_checker_texture) {
					Checker_Texture* texture = static_cast<Checker_Texture*>(material->albedo.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture color odd:\nr: %.3f | g: %.3f | b: %.3f",
					texture->odd->color_value[0], texture->odd->color_value[1], texture->odd->color_value[2]);
					ImGui::Text("Texture color even:\nr: %.3f | g: %.3f | b: %.3f",
					texture->even->color_value[0], texture->even->color_value[1], texture->even->color_value[2]);
				}
				if(material->albedo->id == t_image_texture) {
					Image_Texture* texture = static_cast<Image_Texture*>(material->albedo.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture name: %s", texture->my_file_name);
				}
			}

			if(object->material_ptr->id == t_diffuse_light) {
				Diffuse_Light* material = static_cast<Diffuse_Light*>(object->material_ptr.get());

				material_id = material->id;

				ImGui::Text("Material: %s", material->my_name);

				if(material->emit->id == t_solid_color) {
					Solid_Color* texture = static_cast<Solid_Color*>(material->emit.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture color:\nr: %.3f | g: %.3f | b: %.3f",
					texture->color_value[0], texture->color_value[1], texture->color_value[2]);
				}
				if(material->emit->id == t_checker_texture) {
					Checker_Texture* texture = static_cast<Checker_Texture*>(material->emit.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture color odd:\nr: %.3f | g: %.3f | b: %.3f",
					texture->odd->color_value[0], texture->odd->color_value[1], texture->odd->color_value[2]);
					ImGui::Text("Texture color even:\nr: %.3f | g: %.3f | b: %.3f",
					texture->even->color_value[0], texture->even->color_value[1], texture->even->color_value[2]);
				}
				if(material->emit->id == t_image_texture) {
					Image_Texture* texture = static_cast<Image_Texture*>(material->emit.get());

					texture_id = texture->id;

					ImGui::Text("Texture: %s", texture->my_name);
					ImGui::NewLine();

					ImGui::Text("Texture name: %s", texture->my_file_name);
				}
			}
		}
		template<class T>
		bool object_material_edit(T* object, int &current_material, bool &original_material) {
			ImGui::NewLine();
			if(ImGui::Combo("Material", &current_material, materials, IM_ARRAYSIZE(materials))) {
				change_edit_stop = true;

				original_material = object->material_ptr->id == current_material;
				object->material_ptr->id = static_cast<Material_ID>(current_material);
			}

			if(object->material_ptr->id == t_lambertian) {
				if(!original_material) {
					change_object = true;
					change_edit_stop = true;

					original_material = true;
					object->material_ptr = make_shared<Lambertian>(Color(1,1,1));

					return true;
				}
				Lambertian* material = static_cast<Lambertian*>(object->material_ptr.get());

				static int current_texture = material->albedo->id;
				static bool original_texture = true;

				if(ImGui::Combo("Texture", &current_texture, textures, IM_ARRAYSIZE(textures))) {
					change_edit_stop = true;

					original_texture = material->albedo->id == current_texture;
					material->albedo->id = static_cast<Texture_ID>(current_texture);
				}

				if(material->albedo->id == t_solid_color) {
					if(!original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;
						material->albedo = make_shared<Solid_Color>(Color(1,1,1));

						return true;
					}
					Solid_Color* texture = static_cast<Solid_Color*>(material->albedo.get());

					ImGui::NewLine();
					if(ImGui::ColorEdit3("Color", (float*) &texture->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::InputFloat3("Color", (float*) &texture->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
				}

				if(material->albedo->id == t_checker_texture) {
					if(!original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;
						material->albedo = make_shared<Checker_Texture>(Color(1,1,1), Color(0,0,0));

						return true;
					}
					Checker_Texture* texture = static_cast<Checker_Texture*>(material->albedo.get());

					ImGui::NewLine();
					if(ImGui::ColorEdit3("Color 0", (float*) &texture->odd->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::ColorEdit3("Color 1", (float*) &texture->even->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}

					if(ImGui::InputFloat3("Color 0", (float*) &texture->odd->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::InputFloat3("Color 1", (float*) &texture->even->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
				}

				if(material->albedo->id == t_image_texture) {
					char name[255] = "";

					if(original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;

						Image_Texture* texture = static_cast<Image_Texture*>(material->albedo.get());

						strcpy(name, texture->my_file_name);
					}

					ImGui::NewLine();
					if(ImGui::InputText("Image name/PATH", &name[0], 256) && !ImGui::IsItemActive()) {
						change_object = true;
						change_edit_stop = true;

						if(!original_texture) {
							material->albedo = make_shared<Image_Texture>(name);

							return true;
						}
					}
				}
			}

			if(object->material_ptr->id == t_diffuse_light) {
				if(!original_material) {
					change_object = true;
					change_edit_stop = true;

					original_material = true;
					object->material_ptr = make_shared<Diffuse_Light>(Color(1,1,1));

					return true;
				}
				Diffuse_Light* material = static_cast<Diffuse_Light*>(object->material_ptr.get());

				static int current_texture = material->emit->id;
				static bool original_texture = true;

				if(ImGui::Combo("Texture", &current_texture, textures, IM_ARRAYSIZE(textures))) {
					change_edit_stop = true;

					original_texture = material->emit->id == current_texture;
					material->emit->id = static_cast<Texture_ID>(current_texture);
				}

				if(material->emit->id == t_solid_color) {
					if(!original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;
						material->emit = make_shared<Solid_Color>(Color(1,1,1));

						return true;
					}
					Solid_Color* texture = static_cast<Solid_Color*>(material->emit.get());

					ImGui::NewLine();
					if(ImGui::ColorEdit3("Color", (float*) &texture->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::InputFloat3("Color", (float*) &texture->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
				}

				if(material->emit->id == t_checker_texture) {
					if(!original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;
						material->emit = make_shared<Checker_Texture>(Color(1,1,1), Color(0,0,0));

						return true;
					}
					Checker_Texture* texture = static_cast<Checker_Texture*>(material->emit.get());

					ImGui::NewLine();
					if(ImGui::ColorEdit3("Color 0", (float*) &texture->odd->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::ColorEdit3("Color 1", (float*) &texture->even->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}

					if(ImGui::InputFloat3("Color 0", (float*) &texture->odd->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
					if(ImGui::InputFloat3("Color 1", (float*) &texture->even->color_value)) {
						change_object = true;
						change_edit_stop = true;

						return true;
					}
				}

				if(material->emit->id == t_image_texture) {
					char name[255] = "";

					if(original_texture) {
						change_object = true;
						change_edit_stop = true;

						original_texture = true;

						Image_Texture* texture = static_cast<Image_Texture*>(material->emit.get());

						strcpy(name, texture->my_file_name);
					}

					ImGui::NewLine();
					if(ImGui::InputText("Image name/PATH", &name[0], 256) && !ImGui::IsItemActive()) {
						change_object = true;
						change_edit_stop = true;

						if(!original_texture) {
							material->emit = make_shared<Image_Texture>(name);

							return true;
						}
					}
				}
			}
			return false;
		}

		void object_editor(unsigned int current_object, std::vector<std::shared_ptr<Hittable>>& objects) {
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("%d | %s", current_object,  objects[current_object]->object_name);

			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Edit")) {
					ImGui::TextWrapped("Controls:");

					if(objects[current_object]->id == t_xy_rect) {
						xy_rect* object = static_cast<xy_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start X", &object->x_start)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Start Y", &object->y_start)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End X", &object->x_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End Y", &object->y_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Z", &object->z)) {
							change_object = true;
							change_edit_stop = true;
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						object_material_edit<xy_rect>(object, current_material, original_material);
						ImGui::NewLine();
					}

					if(objects[current_object]->id == t_xz_rect) {
						xz_rect* object = static_cast<xz_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start X", &object->x_start)) {

						}
						if(ImGui::InputFloat("Start Z", &object->z_start)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End X", &object->x_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End Z", &object->z_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Y", &object->y)) {
							change_object = true;
							change_edit_stop = true;
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;


						object_material_edit<xz_rect>(object, current_material, original_material);
						ImGui::NewLine();
					}

					if(objects[current_object]->id == t_yz_rect) {
						yz_rect* object = static_cast<yz_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start Y", &object->y_start)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Start Z", &object->z_start)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End Y", &object->y_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("End Z", &object->z_end)) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("X", &object->x)) {
							change_object = true;
							change_edit_stop = true;
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						object_material_edit<yz_rect>(object, current_material, original_material);
						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_box) {
						Box* object = static_cast<Box*>(objects[current_object].get());

						if(ImGui::InputFloat3("Start Position", &object->start[0])) {
							change_object = true;
							change_edit_stop = true;

							if(change_object && change_edit_stop) {
								object->update();
							}
						}
						if(ImGui::InputFloat3("End Position", &object->end[0])) {
							change_object = true;
							change_edit_stop = true;

							if(change_object && change_edit_stop) {
								object->update();
							}
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						if(object_material_edit<Box>(object, current_material, original_material)) {
							if(change_object && change_edit_stop) {
								object->update();
							}
						}
					}

					if (objects[current_object]->id == t_obj) {
						Obj* object = static_cast<Obj*>(objects[current_object].get());

						// ImGui::InputFloat3("Position:", &object->postion);

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						object_material_edit<Obj>(object, current_material, original_material);
						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_sphere_moving) {
						Sphere_moving* object = static_cast<Sphere_moving*>(objects[current_object].get());

						if(ImGui::InputFloat3("Position C0", &object->center_0[0])) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat3("Position C1", &object->center_1[0])) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Radius", &object->radius)) {
							change_object = true;
							change_edit_stop = true;
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						object_material_edit<Sphere_moving>(object, current_material, original_material);
						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_sphere) {
						Sphere* object = static_cast<Sphere*>(objects[current_object].get());

						if(ImGui::InputFloat3("Position", &object->center[0])) {
							change_object = true;
							change_edit_stop = true;
						}
						if(ImGui::InputFloat("Radius", &object->radius)) {
							change_object = true;
							change_edit_stop = true;
						}

						static int current_material = object->material_ptr->id;
						static bool original_material = true;

						object_material_edit<Sphere>(object, current_material, original_material);
						ImGui::NewLine();
					}
					ImGui::Separator();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					unsigned int object_id   = 0;
					unsigned int material_id = 0;
					unsigned int texture_id  = 0;

					if(objects[current_object]->id == t_xy_rect) {
						xy_rect* object = static_cast<xy_rect*>(objects[current_object].get());

						ImGui::Text("Position:\nx-start: %.3f\ny-start: %.3f\nx-end: %.3f\ny-end: %.3f\nz: %.3f",
						object->x_start, object->y_start, object->x_end, object->y_end, object->z);

						object_details<xy_rect>(object_id, material_id, texture_id, object);
					}
					if(objects[current_object]->id == t_xz_rect) {
						xz_rect* object = static_cast<xz_rect*>(objects[current_object].get());

						ImGui::Text("Position:\nx-start: %.3f\nz-start: %.3f\nx-end: %.3f\nz-end: %.3f\ny: %.3f",
						object->x_start, object->z_start, object->x_end, object->z_end, object->y);

						object_details<xz_rect>(object_id, material_id, texture_id, object);
					}
					if(objects[current_object]->id == t_yz_rect) {
						yz_rect* object = static_cast<yz_rect*>(objects[current_object].get());

						ImGui::Text("Position:\ny-start: %.3f\nz-start: %.3f\ny-end: %.3f\nz-end: %.3f\nx: %.3f",
						object->y_start, object->z_start, object->y_end, object->z_end, object->x);

						object_details<yz_rect>(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_box) {
						Box* object = static_cast<Box*>(objects[current_object].get());

						ImGui::Text("Start Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->start[0], object->start[1], object->start[2]);
						ImGui::Text("End Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->end[0], object->end[1], object->end[2]);

						object_details<Box>(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_obj) {
						Obj* object = static_cast<Obj*>(objects[current_object].get());

						object_details<Obj>(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_sphere_moving) {
						Sphere_moving* object = static_cast<Sphere_moving*>(objects[current_object].get());

						ImGui::Text("C0 Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center_0[0], object->center_0[1], object->center_0[2]);
						ImGui::Text("C1 Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center_1[0], object->center_1[1], object->center_1[2]);
						ImGui::Text("Radius: %.3f", object->radius);

						object_details<Sphere_moving>(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_sphere) {
						Sphere* object = static_cast<Sphere*>(objects[current_object].get());

						ImGui::Text("Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center[0], object->center[1], object->center[2]);
						ImGui::Text("Radius: %.3f", object->radius);

						object_details<Sphere>(object_id, material_id, texture_id, object);
					}
					ImGui::NewLine();
					ImGui::Separator();

					ImGui::Text("Object   ID: %d", object_id);
					ImGui::Text("Material ID: %d", material_id);
					ImGui::Text("Texture  ID: %d", texture_id);

					ImGui::Separator();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();

			bool add = ImGui::Button("ADD");
			if (add) {
				ImGui::OpenPopup("my_select_popup");
			}
			const char* objects_names[] = { "Rect-xy", "Rect-xz", "Rect-yz", "Box", "Obj", "Moving-sphere", "Sphere" };

			if (ImGui::BeginPopup("my_select_popup")) {

				ImGui::Text("Objects");
				ImGui::Separator();

				for (int i = 0; i < IM_ARRAYSIZE(objects_names); i++) {
					if (ImGui::Selectable(objects_names[i])) {
						if(i == 0) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<xy_rect>(0, 0, 0, 0, 0, make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 1) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<xz_rect>(0, 0, 0, 0, 0, make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 2) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<yz_rect>(0, 0, 0, 0, 0, make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 3) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<Box>(Point(0, 0, 0), Point(0, 0, 0), make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 4) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<Obj>(embree, "", "", make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 5) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<Sphere_moving>(Point(0, 0, 0), Point(0, 0, 0), 0, 0, 0, make_shared<Lambertian>(Color(1,1,1))));
						}
						if(i == 6) {
							change_object_list = true;
							change_edit_stop = true;
							world.add(make_shared<Sphere>(Point(0, 0, 0), 0, make_shared<Lambertian>(Color(1,1,1))));
						}
					}
				}
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			bool copy = ImGui::Button("COPY");

			if(copy) {
				// TODO: Add copy function
			}

			ImGui::SameLine(180);
			bool remove = ImGui::Button("REMOVE");
			if(ImGui::IsItemActive()) { change_edit_stop = true; }

			if(remove) {
				change_object_list = true;
				if(edit_allowed) {
					world.remove(current_object);
				}
			}

			ImGui::SameLine(240);
			bool remove_all = ImGui::Button("REMOVE ALL");
			if(ImGui::IsItemActive()) { change_edit_stop = true; }

			if(remove_all) {
				change_object_list = true;

				if(edit_allowed) {
					while(!world.objects.empty()) {
						world.remove(current_object);
					}
				}
			}
			ImGui::EndGroup();
		}

		void file_edit() {
			if(show_file_edit) {
				ImGui::Begin("Object edit", &show_file_edit);
					static int selected = 0;
					static std::vector<shared_ptr<Hittable>> &my_objects = world.objects;
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

					object_editor(selected, my_objects);
				ImGui::End();
			}
		}

		void debug(ImGuiWindowFlags &window_flags) {
			if(show_debug) {
				ImGui::Begin("Debug menu", &show_debug, window_flags);
					ImGui::Separator();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "ImGui Bools:");

					ImGui::Text("show_file_edit:   %s", show_file_edit ? "true" : "false");
					ImGui::Text("show_details:     %s", show_details ? "true" : "false");
					ImGui::Text("show_camera:      %s", show_camera ? "true" : "false");
					ImGui::Text("show_render:      %s", show_render ? "true" : "false");
					ImGui::Text("show_debug:       %s", show_debug ? "true" : "false");
					ImGui::Text("show_demo:        %s", show_demo ? "true" : "false");
					ImGui::Text("show_exit:        %s", show_exit ? "true" : "false");
					ImGui::Text("show_overlay:     %s", show_overlay ? "true" : "false");

					ImGui::NewLine();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "Render Bools:");

					ImGui::Text("change_multithreading: %s", change_multithreading ? "true" : "false");
					ImGui::Text("change_single_cast:    %s", change_single_cast ? "true" : "false");
					ImGui::Text("change_close_window:   %s", change_close_window ? "true" : "false");
					ImGui::Text("change_object_list:    %s", change_object_list ? "true" : "false");
					ImGui::Text("change_object:         %s", change_object ? "true" : "false");
					ImGui::Text("change_static:         %s", change_static ? "true" : "false");
					ImGui::Text("change_scene:          %s", change_scene ? "true" : "false");
					ImGui::Text("change_camera:         %s", change_camera ? "true" : "false");
					ImGui::Text("change_clear:          %s", change_clear ? "true" : "false");
					ImGui::Text("change_default:        %s", change_default ? "true" : "false");
					ImGui::Text("change_edit_stop:      %s", change_edit_stop ? "true" : "false");
					ImGui::Text("change_force_stop:     %s", change_force_stop ? "true" : "false");

					ImGui::Text("edit_allowed:          %s", edit_allowed ? "true" : "false");

					ImGui::NewLine();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "Object count:");
					ImGui::Text("Object count: %d", world.objects.size());

					ImGui::NewLine();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "Threads:");
					ImGui::Text("Max Threads: %d", MAX_NUMBER_OF_THREADS);
					ImGui::Text("Working Threads: %d", change_multithreading ? MAX_NUMBER_OF_THREADS : 1);

					ImGui::NewLine();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "Buckets:");
					ImGui::Text("Total buckets: %d", total_buckets);
					ImGui::Text("Bucket size: %d", bucket_size);
					ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.3f, 1.0f), "Buckets in: %d", buckets_in_counter.load());

					ImGui::NewLine();
					ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "Scene:");
					ImGui::Text("Scene aspect ratio: %f", aspect_ratio);
					ImGui::Text("Scene width: %d", image_width);
					ImGui::Text("Scene height: %d", image_height);
					ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.3f, 1.0f), "Scenes in: %d", scenes_in_counter.load());

					ImGui::Separator();
				ImGui::End();
			}
		}

		void reset_scene() {
			fov        = default_fov;
			max_depth  = default_max_depth;
			lookfrom   = default_lookfrom;
			lookat     = default_lookat;
			background = default_background;
		}
		void clear_scene() {
			max_depth  = default_max_depth;
			background = default_background;
		}

		void setStyle() {
			/* ImGui Style */
			ImGuiStyle& style = ImGui::GetStyle();

			style.FrameBorderSize = 1.0f;
			style.WindowBorderSize = 0.0f;

			ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel);

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
		const char* materials[3] = { "None", "Lambertian", "Diffuse-Light" };
		const char* textures[4] = { "None", "Solid-Color", "Checker-Texture", "Image-Texture" };

		bool show_file_edit     = false;
		bool show_details       = false;
		bool show_camera        = false;
		bool show_render        = false;
		bool show_debug         = false;
		bool show_demo          = false;
		bool show_exit          = false;
		bool show_overlay       = true;
};
