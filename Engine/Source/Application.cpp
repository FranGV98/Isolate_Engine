#include "Application.h"

#include <functional>			//function pointers
#include <vector>				//Vector 
#include <algorithm>			//for_each()
#include <memory>				//Smart pointers

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	scene_intro = new ModuleSceneIntro();
	camera = new ModuleCamera3D();
	GUI = new ModuleGUI();
	Import_3D = new Module3DImport();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(GUI);
	AddModule(Import_3D);

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
