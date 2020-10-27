#include "Globals.h"
#include "Application.h"

#include "glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#pragma comment (lib, "glew/libx86/glew32.lib")

ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
}
ModuleGUI::~ModuleGUI()
{}

bool ModuleGUI::Init()
{
	LOG("Creating GUI");
	bool ret = true;

	//Window size

	height_int = App->window->Height();
	width_int = App->window->Width();

	glewInit(); //Glew

	 // Setup Dear ImGui context
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(0);

	//Color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//States
	show_demo_window = false;
	show_about_window = false;
	show_config_window = false;
	show_console_window = true;

	return ret;
};

update_status ModuleGUI::PreUpdate(float dt)
{
	ShortKeys();
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return  UPDATE_STOP;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
			return  UPDATE_STOP;
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	//DEMO WINDOW
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	//TOP BAR MENU:
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Quit", "ESC")) 
				return UPDATE_STOP; 

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("Configuration", NULL, &show_config_window);
			//if (ImGui::MenuItem("Configuration"))
			//	show_config_window = !show_config_window;

			if (ImGui::MenuItem("OpenGL")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Console", "F6", &show_console_window);
	

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo"))
				show_demo_window = !show_demo_window;

			if (ImGui::MenuItem("Documentation")) 
				RequestBrowser("https://github.com/FranGV98/Isolate_Engine/blob/master/README.md");

			if (ImGui::MenuItem("Latest Release")) 
				RequestBrowser("https://github.com/FranGV98/Isolate_Engine");

			if (ImGui::MenuItem("Report a bug")) 
				RequestBrowser("https://github.com/FranGV98/Isolate_Engine");

			if (ImGui::MenuItem("About")) show_about_window = !show_about_window;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
		ImGui::End();
	}

	//FPS & MS GRAPH
	if (fps_log.size() <= 100)
		fps_log.push_back(ImGui::GetIO().Framerate);
	else
		fps_log.erase(fps_log.begin());

	if (ms_log.size() <= 100)
		ms_log.push_back(App->dt * 1000);
	else
		ms_log.erase(ms_log.begin());

	//CONFIG WINDOW:
	if (show_config_window)
	{
		if (!ImGui::Begin("Configuration", &show_config_window))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Text("Options");
			if (ImGui::CollapsingHeader("Application"))
			{
				sprintf_s(title, 25, "Framerate %1.f", fps_log[fps_log.size() - 1]);
				ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

				sprintf_s(title, 25, "Milliseconds %1.f", ms_log[ms_log.size() - 1]);
				ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
				
				ImGui::Checkbox("Limit FPS", &enable_fps_limit);
				ImGui::SameLine();
				ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
				int max_fps = 1000 / App->ms_limit;
				if (ImGui::SliderInt("Max FPS", &max_fps, 10, 144)) 
					App->ms_limit = 1000 / max_fps;
			}
			if (ImGui::CollapsingHeader("Window"))
			{
				ImGui::SliderFloat("Brightness", &bright_float, 0.0f, 1.0f);
				SDL_SetWindowBrightness(App->window->window, bright_float);

				if (ImGui::Combo("Resolution", &current_resolution, resolution, IM_ARRAYSIZE(resolution)))
				{
					switch (current_resolution)
					{
					case 0:
						width_int = 3840;
						height_int = 2160;
						break;

					case 1:
						width_int = 2560;
						height_int = 1440;
						break;

					case 2:
						width_int = 1920;
						height_int = 1080;
						break;

					case 3:
						width_int = 1290;
						height_int = 720;
						break;

					case 4:
						width_int = 720;
						height_int = 480;
						break;

					}
					SDL_SetWindowSize(App->window->window, width_int, height_int);
				}			

				if (ImGui::Checkbox("Fullscreen", &fullscreen))
					if (fullscreen)
						SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
					else
						SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_MINIMIZED);
			}
			if (ImGui::CollapsingHeader("Renderer"))
			{
				if (ImGui::Checkbox("Wireframe", &wireframe))
					if (wireframe == true)
					{
						LOG("Wireframe ON");
					}
					else
						LOG("Wireframe OFF");

				ImGui::Checkbox("GL_DEPTH", &gl_depth);
				ImGui::Checkbox("GL_CULL_FACE", &gl_cull_face);
				ImGui::Checkbox("GL_LIGHT", &gl_light);
				ImGui::Checkbox("GL_COLOR_MATERIAL", &gl_color_mat);
				ImGui::Checkbox("Draw normals direction", &draw_normals_dir);
			}
			if (ImGui::CollapsingHeader("Hardware"))
			{
				ImGui::Text("CPU: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d (Cache: %d kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

				ImGui::Text("RAM: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", SDL_GetSystemRAM());
			}

			ImGui::End();
		}
	}
		
	//CONSOLE 
	if (show_console_window)
	{
		if (!ImGui::Begin("Console", &show_console_window))
		{
			ImGui::End();
		}
		else
		{
			std::vector<char*>::iterator item = logs.begin();

			for (item; item != logs.end(); ++item)
			{
				ImGui::TextUnformatted((*item));
			}

			if (update_scroll)
			{
				ImGui::SetScrollHere(1.0f);
				update_scroll = false;
			}

			ImGui::End();
		}

	}

	//ABOUT WINDOW
	if (show_about_window)
	{
		if (!ImGui::Begin("About", &show_about_window))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Text("ISOLATE ENGINE \n \nEngine made by mamadisima people");
			ImGui::Spacing();
			ImGui::Text("Developed by:");

			sprintf(label, "Fran Guerrero");
			if (ImGui::Selectable(label, true))	
				RequestBrowser("https://github.com/FranGV98");		
			sprintf(label, "Guillem Turmo");
			if (ImGui::Selectable(label, true))	
				RequestBrowser("https://github.com/Turmo11");

			ImGui::Separator();
			ImGui::Text("3rd Party Libraries used:");
			ImGui::BulletText("SDL");
			ImGui::BulletText("Glew");
			ImGui::BulletText("ImGui");
			ImGui::BulletText("MathGeoLib");
			ImGui::BulletText("Assimp");
			ImGui::BulletText("OpenGL");
			ImGui::Separator();
			ImGui::Text("License: \n \n(Mucho texto)");

			ImGui::End();
		}
	}

	//RENDERER
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w); //Color entorno 3D (Skybox)
	//glClear(GL_COLOR_BUFFER_BIT); //Canvas COLOR
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Update and Render additional Platform Windwo
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
	
	//SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	ClearConsole();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

//Browser function
void ModuleGUI::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);

}

void ModuleGUI::ConsoleLog(char* log)
{
	logs.push_back(strdup(log));
	update_scroll = true;
}

void ModuleGUI::ClearConsole()
{
	for (int i = 0; i < logs.size(); ++i)					
	{
		free(logs[i]);
	}

	logs.clear();
}

void ModuleGUI::ShortKeys()
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_STATE::KEY_DOWN)
	{
		show_console_window = !show_console_window;
	}
}