#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"

#include "glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
}
ModuleGUI::~ModuleGUI()
{}

bool ModuleGUI::Init()
{
	LOG("Creating GUI");
	bool ret = true;

	//glewInit(); //Glew

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//ImGui::StyleColorsDark();

	//// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	//ImGuiStyle& style = ImGui::GetStyle();
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	style.WindowRounding = 7.0f;
	//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	//}
	//// Our state
	//show_demo_window = true;

	//// Setup Platform/Renderer bindings
	//ImGui_ImplSDL2_InitForOpenGL(window, gl_context);

	return ret;
};

update_status ModuleGUI::Update(float dt)
{
	//ImGui::Begin("Hello, world!");

	//ImGui::End();

	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	////Render
	//ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(2, 2, 2, 2);
	//glClear(GL_COLOR_BUFFER_BIT);
	////ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//// Update and Render additional Platform Windows
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	//	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	//	ImGui::UpdatePlatformWindows();  
	//	ImGui::RenderPlatformWindowsDefault();
	//	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	//}

	//SDL_GL_SwapWindow(window);

	return UPDATE_CONTINUE;
}


bool ModuleGUI::CleanUp()
{
	return true;
}