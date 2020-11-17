#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
//#include "MathGeoLib/src/MathGeoLib.h"

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#pragma comment (lib, "glew/libx86/glew32.lib")

GUI::GUI(bool start_enabled) : Module(start_enabled)
{

}
GUI::~GUI()
{}

bool GUI::Init()
{
	LOG("Creating GUI");
	bool ret = true;

	//Window size

	height_int = App->window->Height();
	width_int = App->window->Width();

	glewInit(); //Glew

	 // Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
	tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	//States
	show_demo_window = false;
	show_about_window = false;
	show_config_window = false;
	show_console_window = true;
	show_hierarchy_window = true;
	show_inspector_window = true;

	

	return ret;
};

update_status GUI::PreUpdate(float dt)
{
	ShortKeys();

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

	return UPDATE_CONTINUE;
}

update_status GUI::PostUpdate(float dt)
{
	SetDocking(ImGuiWindowFlags_MenuBar);	

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
			ImGui::MenuItem("Hierarchy","F1", &show_hierarchy_window);
			ImGui::MenuItem("Inspector", "F2", &show_inspector_window);
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
				
				ImGui::Checkbox("Limit FPS", &App->enable_fps_limit);
				ImGui::SameLine();
				ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
				 max_fps = 1000 / App->ms_limit;
				if (ImGui::SliderInt("Max FPS", &max_fps, 10, 120)) 
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
			if (ImGui::CollapsingHeader("Input"))
			{
				ImGui::Text("MOUSE POSITION    ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "X: %d Y: %d", App->input->GetMouseX(), App->input->GetMouseY());

				ImGui::Text("MOUSE MOTION    ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "X: %d Y: %d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			}
			if (ImGui::CollapsingHeader("Hardware & Software"))
			{
				ImGui::Text("CPU: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Cores (Cache: %d kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

				ImGui::Text("RAM: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", SDL_GetSystemRAM());
				
				ImGui::Text("Brand: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", glGetString(GL_VENDOR));

				ImGui::Text("Model: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", glGetString(GL_RENDERER));

				int budget;
				glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &budget);
				ImGui::Text("VRAM Budget: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", budget / 1024);

				int usage;
				glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &usage);
				ImGui::Text("VRAM Usage: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", usage / 1024);

				int available;
				glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
				ImGui::Text("VRAM Avaliable: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", available / 1024);

				int reserved;
				glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &reserved);
				ImGui::Text("VRAM Reserved: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "%d Mb", reserved / 1024);

				ImGui::Text("SDL version: ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "2.0.12");
			}

			ImGui::End();
		}
	}
		
	//CONSOLE WINDOW
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

			if (ImGui::Button("Clear"))
			{
				ClearConsole();
			}

			ImGui::End();
		}

	}

	//HIERARCHY WINDOW
	if (show_hierarchy_window)
	{
		ImGui::Begin("Hierarchy", &show_hierarchy_window);

		for (std::vector<GameObject*>::iterator item = App->gobjects_manager->game_objects.begin(); item != App->gobjects_manager->game_objects.end(); item++)
		{			
			ObtainGameObjects(*item);
		}

		ImGui::End();
	}	
	
	//INSPECTOR WINDOW
	if (show_inspector_window)
	{
		ImGui::Begin("Inspector", &show_inspector_window);
		if (selected_gameobject != nullptr)
		{
			//set object active
			bool active_go = selected_gameobject->isActive();
			if (ImGui::Checkbox(" ", &active_go))
			{
				selected_gameobject->SetActive(active_go);
			}
			
			ImGui::SameLine();
			static char name_buff[32];
			strcpy(name_buff, selected_gameobject->GetName());
			if (ImGui::InputText("name", name_buff, IM_ARRAYSIZE(name_buff), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				selected_gameobject->SetName(name_buff);
			}
			ImGui::Spacing();
			if (ImGui::CollapsingHeader("Transform"))
			{   
				float3 f3_pos = selected_gameobject->transform->GetPosition();
				float f_pos[3] = { f3_pos.x, f3_pos.y, f3_pos.z };
				if (ImGui::DragFloat3("Position", f_pos, 0.5f, -999999, 999999, "%.2f", NULL))
				{
					selected_gameobject->transform->SetPosition(float3(f_pos[0], f_pos[1], f_pos[2]));
				}

				Quat f3_rot = selected_gameobject->transform->GetRotation();
				float f_rot[3] = { f3_rot.x, f3_rot.y, f3_rot.z };
				if (ImGui::DragFloat3("Rotation", f_rot, 0.5f, -999999, 999999, "%.2f", NULL))
				{
					selected_gameobject->transform->SetRotation(Quat(f_rot[0], f_rot[1], f_rot[2], 0));
				}

				float3 f3_scale = selected_gameobject->transform->GetScale();
				float f_scale[3] = { f3_scale.x, f3_scale.y, f3_scale.z };
				if (ImGui::DragFloat3("Scale", f_scale, 0.5f, -999999, 999999, "%.2f", NULL))
				{
					selected_gameobject->transform->SetScale(float3(f_scale[0], f_scale[1], f_scale[2]));
				}
			}
			if (ImGui::CollapsingHeader("Mesh"))
			{
				ImGui::Text("Path mesh:");
				//ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", );
			}
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Text("Path Texture:");
				//ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", );
			}
		}
		if (ImGui::CollapsingHeader("Create Objects"))
		{
			ImGui::Text("GEOMETRY");
			ImGui::Separator();

			if (ImGui::Button("Sphere"))
			{
				App->gobjects_manager->CreateGameObject("Sphere", selected_gameobject);
				App->renderer3D->ImportMesh("Assets/models/Sphere.FBX");
			}
			ImGui::SameLine();
			if (ImGui::Button("Box"))
			{
				App->gobjects_manager->CreateGameObject("Box", selected_gameobject);
				App->renderer3D->ImportMesh("Assets/models/Box.FBX");
			}
			ImGui::SameLine();
			if (ImGui::Button("Cone"))
			{
				App->gobjects_manager->CreateGameObject("Cone", selected_gameobject);
				App->renderer3D->ImportMesh("Assets/models/Cone.FBX");
			}
			ImGui::SameLine();
			if (ImGui::Button("Cylinder"))
			{
				App->gobjects_manager->CreateGameObject("Cylinder", selected_gameobject);
				App->renderer3D->ImportMesh("Assets/models/Cylinder.FBX");
			}

			ImGui::Separator();
		}
		ImGui::End();
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
			sprintf(label, "GNU General Public License v3.0 (click to view)");
			if (ImGui::Selectable(label, true))
				RequestBrowser("https://github.com/FranGV98/Isolate_Engine/blob/master/LICENSE.md");

			ImGui::End();
		}
	}

	ImGui::End(); //Docking

	//RENDERER
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w); //Color entorno 3D (Skybox)
	//glClear(GL_COLOR_BUFFER_BIT); //Canvas COLOR
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Update and Render additional Platform Windwow
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	//}

	return UPDATE_CONTINUE;
}

bool GUI::CleanUp()
{
	ClearConsole();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

//Browser function
void GUI::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);

}

void GUI::ConsoleLog(char* log)
{
	logs.push_back(strdup(log));
	update_scroll = true;
}

void GUI::ClearConsole()
{
	for (int i = 0; i < logs.size(); ++i)					
	{
		free(logs[i]);
	}

	logs.clear();
}

void GUI::ShortKeys()
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN)
	{
		show_hierarchy_window = !show_hierarchy_window;
	}if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_STATE::KEY_DOWN)
	{
		show_inspector_window = !show_inspector_window;
	}if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_STATE::KEY_DOWN)
	{
		show_console_window = !show_console_window;
	}
}

bool GUI::SetDocking(ImGuiWindowFlags window_flags)
{
	bool p_open = true;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.

		ImGuiViewport* viewport = ImGui::GetWindowViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);

	ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	return p_open;
}

void GUI::ObtainGameObjects(GameObject* go)
{
	//Defines color (grey inactive / pink selected)
	ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (!go->isActive())														// If the given game object is not active, the text of the tree node will be displayed in GREY.
	{
		color = { 0.5f, 0.5f, 0.5f, 1.0f }; //Grey
	}
	if (go == selected_gameobject && go->isActive())
	{
		color = { 1.0f, 0.0f, 1.0f, 1.0f }; //Pink
	}

	ImGui::PushStyleColor(ImGuiCol_Text, color);

	ImGuiTreeNodeFlags node_flags = tree_flags;

	//ImGuiTreeNodeFlags_Leaf = Non collapsable + no arrow
	if (go->children.empty())
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf; 
	}

	if (go == selected_gameobject)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(go->GetName(), node_flags))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			SelectGameObject(go);
		}
		if (ImGui::BeginDragDropSource())												
		{
			ImGui::SetDragDropPayload("DRAGGED_NODE", go, sizeof(GameObject));	
			ImGui::Text("Dragging %s", go->GetName());				
			dragged_object = go;

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGGED_NODE"))
			{
				if (go->parent != dragged_object)
				{
					go->InsertChild(dragged_object);
					dragged_object = nullptr;
				}
				else
				{
					LOG("Can't make child of its own child");
				}
				
			}

			ImGui::EndDragDropTarget();
		}

		for (std::vector<GameObject*>::iterator item = go->children.begin(); item != go->children.end(); item++)
		{
			GameObject* current_go = *item;
			ObtainGameObjects(current_go);
		}
		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}

void GUI::SelectGameObject(GameObject* selected_object)
{
	if (selected_object != selected_gameobject)
	{
		selected_gameobject = selected_object;
	}
}