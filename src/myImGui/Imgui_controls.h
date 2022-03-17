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
				ImGui::MenuItem("Settings", NULL, &show_settings);
				ImGui::MenuItem("Camera", NULL, &show_camera);
				ImGui::MenuItem("Render info", NULL, &show_render_info);
				ImGui::Separator();
				ImGui::MenuItem("ImGui menu", "Ctrl+D", &show_demo);
				if(ImGui::MenuItem("Exit", "Alt+F4")) {
					show_exit = true;
				} else { show_exit = false; }
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Edit")) {
				ImGui::MenuItem("File edit", NULL, &show_file_edit);
				ImGui::EndMenu();
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

				reset_scene();
			} else { change_default = false; }

			ImGui::SameLine(ImGui::GetWindowWidth()-199);
			if(ImGui::MenuItem("Clear")) {
				change_clear = true;

				clear_scene();
			} else { change_clear = false; }

			ImGui::SameLine(ImGui::GetWindowWidth()/2);
			if(change_stop || change_remove_stop) { ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.3f, 1.0f), "STOPPED"); }
			else { ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.3f, 1.0f), "RUNNING"); }

			ImGui::EndMainMenuBar();

			file_edit();
			settings();
			camera_control();
			render_info();
			exit();

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
		}

		void camera_control() {
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
		}
		
		void render_info() {
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
		}

		void xy_rect_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, xy_rect *object) {

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

		void xz_rect_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, xz_rect *object) {

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

		void yz_rect_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, yz_rect *object) {

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

		void obj_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, Obj* object) {

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

		void sphere_moving_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, Sphere_moving *object) {

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

		void sphere_details(unsigned int &object_id,unsigned int &material_id, unsigned int &texture_id, Sphere* object) {

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

		void object_editor(unsigned int current_object, std::vector<std::shared_ptr<Hittable>>& objects) {
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("%d | %s", current_object,  objects[current_object]->object_name);

			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Edit")) {
					ImGui::TextWrapped("Controls:");

					if(objects[current_object]->id == t_xy_rect) {
						xy_rect* object = static_cast<xy_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start X:", &object->x_start) ||
						   ImGui::InputFloat("Start Y:", &object->y_start) ||
						   ImGui::InputFloat("End X:", &object->x_end) ||
						   ImGui::InputFloat("End Y:", &object->y_end) ||
						   ImGui::InputFloat("Z:", &object->z)) {
							change_object = true;
						} else { change_object = false; };

						ImGui::NewLine();
					}

					if(objects[current_object]->id == t_xz_rect) {
						xz_rect* object = static_cast<xz_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start X:", &object->x_start) ||
						   ImGui::InputFloat("Start Z:", &object->z_start) ||
						   ImGui::InputFloat("End X:", &object->x_end) ||
						   ImGui::InputFloat("End Z:", &object->z_end) ||
						   ImGui::InputFloat("Y:", &object->y)) {
							change_object = true;
						}
						else { change_object = false; };

						ImGui::NewLine();
					}

					if(objects[current_object]->id == t_yz_rect) {
						yz_rect* object = static_cast<yz_rect*>(objects[current_object].get());

						if(ImGui::InputFloat("Start Y:", &object->y_start) ||
						   ImGui::InputFloat("Start Z:", &object->z_start) ||
						   ImGui::InputFloat("End Y:", &object->y_end) ||
						   ImGui::InputFloat("End Z:", &object->z_end) ||
						   ImGui::InputFloat("X:", &object->x)) {
							change_object = true;
						}
						else { change_object = false; };

						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_obj) {
						Obj* object = static_cast<Obj*>(objects[current_object].get());

						// ImGui::InputFloat3("Position:", &object->postion);

						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_sphere_moving) {
						Sphere_moving* object = static_cast<Sphere_moving*>(objects[current_object].get());

						if(ImGui::InputFloat3("Position C0:", &object->center_0[0]) ||
						   ImGui::InputFloat3("Position C1:", &object->center_1[0]) ||
						   ImGui::InputFloat("Radius:", &object->radius)) {
							change_object = true;
						}
						else { change_object = false; }

						ImGui::NewLine();
					}

					if (objects[current_object]->id == t_sphere) {
						Sphere* object = static_cast<Sphere*>(objects[current_object].get());

						if(ImGui::InputFloat3("Position:", &object->center[0]) ||
						   ImGui::InputFloat("Radius:", &object->radius)) {
							change_object = true;
						}
						else { change_object = false; }
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

						xy_rect_details(object_id, material_id, texture_id, object);
					}
					if(objects[current_object]->id == t_xz_rect) {
						xz_rect* object = static_cast<xz_rect*>(objects[current_object].get());

						ImGui::Text("Position:\nx-start: %.3f\nz-start: %.3f\nx-end: %.3f\nz-end: %.3f\ny: %.3f",
						object->x_start, object->z_start, object->x_end, object->z_end, object->y);

						xz_rect_details(object_id, material_id, texture_id, object);
					}
					if(objects[current_object]->id == t_yz_rect) {
						yz_rect* object = static_cast<yz_rect*>(objects[current_object].get());

						ImGui::Text("Position:\ny-start: %.3f\nz-start: %.3f\ny-end: %.3f\nz-end: %.3f\nx: %.3f",
						object->y_start, object->z_start, object->y_end, object->z_end, object->x);

						yz_rect_details(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_obj) {
						Obj* object = static_cast<Obj*>(objects[current_object].get());
						obj_details(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_sphere_moving) {
						Sphere_moving* object = static_cast<Sphere_moving*>(objects[current_object].get());

						ImGui::Text("C0 Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center_0[0], object->center_0[1], object->center_0[2]);
						ImGui::Text("C1 Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center_1[0], object->center_1[1], object->center_1[2]);
						ImGui::Text("Radius: %.3f", object->radius);

						sphere_moving_details(object_id, material_id, texture_id, object);
					}

					if (objects[current_object]->id == t_sphere) {
						Sphere* object = static_cast<Sphere*>(objects[current_object].get());

						ImGui::Text("Position:\nx: %.3f | y: %.3f | z: %.3f",
						object->center[0], object->center[1], object->center[2]);
						ImGui::Text("Radius: %.3f", object->radius);

						sphere_details(object_id, material_id, texture_id, object);
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
			bool remove = ImGui::Button("REMOVE");
			if(ImGui::IsItemHovered()) { change_remove_stop = true; }
			else { change_remove_stop = false; }

			if(remove) {
				change_object_remove = true;

				world.remove(current_object);
			} else { change_object_remove = false; }
			ImGui::EndGroup();
		}

		void file_edit() {
			if(show_file_edit) {
				ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("File edit")) {

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

				}
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
		bool show_exit        = false;
		bool show_overlay     = true;
};
