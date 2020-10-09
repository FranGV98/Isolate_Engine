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

	return ret;
};

update_status ModuleGUI::PreUpdate(float dt)
{
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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC")) 
				return UPDATE_STOP; 

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Configuration"))
				show_config_window = !show_config_window;

			if (ImGui::MenuItem("OpenGL")) {}

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

	//FPS GRAPH
	if (fps_log.size() <= 100)
		fps_log.push_back(ImGui::GetIO().Framerate);
	else
		fps_log.erase(fps_log.begin());

	if (ms_log.size() <= 100)
		ms_log.push_back(frame_time.Read());
	else
		ms_log.erase(ms_log.begin());

	//CONFIG WINDOW:
	if (show_config_window) {
		ImGui::Begin("Configuration");
		ImGui::Text("Options");
		if (ImGui::CollapsingHeader("Application"))
		{
			sprintf_s(title, 25, "Framerate %1.f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %1.f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			bright_int = 0;
			ImGui::SliderInt("Brightness", &bright_int, 0, 10);			
			
			height_int = 0;
			ImGui::SliderInt("Height", &height_int, 0, 10);
			
			width_int = 0;
			ImGui::SliderInt("Width", &width_int, 0, 10);

			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				if(fullscreen)
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
				else
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_MINIMIZED);
		}
		ImGui::End();
	}

	//ABOUT WINDOW
	if (show_about_window)
	{
		ImGui::Begin("About");
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