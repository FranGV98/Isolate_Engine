#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Renderer3D.h"
#include "Camera3D.h"
#include "GUI.h"
#include "FileSystem.h"
#include "GameObjectManager.h"

#include <list>
#include <memory>

class Application
{
public:
	Window*			window = nullptr;
	Input*			input = nullptr;
	Scene*		scene = nullptr;
	Renderer3D*		renderer3D = nullptr;
	Camera3D*			camera = nullptr;
	GUI*					gui = nullptr;
	FileSystem*				file_system = nullptr;
	GameObjectManager*		gobjects_manager = nullptr;

	bool debug;
	bool renderPrimitives;

public:

	Timer	ms_timer;
	float	dt;
	float	fps;
	float	ms_limit;
	bool	enable_fps_limit = true;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	std::vector<Module*> modulelist;			
};