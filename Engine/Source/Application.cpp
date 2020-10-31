#include "Application.h"

#include <functional>			//function pointers
#include <algorithm>			//for_each()
#include <memory>				//Smart pointers

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window				= new ModuleWindow();
	input				= new ModuleInput();
	scene_intro			= new ModuleSceneIntro();
	renderer3D			= new ModuleRenderer3D();
	camera				= new ModuleCamera3D();
	GUI					= new ModuleGUI();
	file_system			= new FileSystem();
	gobjects_manager	= new GameObjectManager();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(file_system);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(GUI);
	AddModule(gobjects_manager);

	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::iterator item = modulelist.begin();
	
	while (item != modulelist.end())
	{
		delete (*item);
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	std::vector<Module*>::iterator item = modulelist.begin();

	while(item != modulelist.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}
	
	LOG("Application Start --------------");
	item = modulelist.begin();

	while(item != modulelist.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	ms_limit = 1000 / 60;

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------

void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (enable_fps_limit)
	{
		int last_frame_ms = ms_timer.Read();

		if (last_frame_ms < ms_limit)
		{
			SDL_Delay(ms_limit - last_frame_ms);
		}
	}	
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>::iterator item = modulelist.begin();
	
	while(item != modulelist.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = modulelist.begin();

	while(item != modulelist.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = modulelist.begin();

	while(item != modulelist.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::vector<Module*>::iterator item = modulelist.begin();

	while(item != modulelist.end() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	modulelist.push_back(mod);
}

Application* App = nullptr;
