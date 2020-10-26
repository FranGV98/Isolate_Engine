#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUI.h"
//#include "Module3DImport.h"

#include <list>
#include <vector>
#include <memory>

class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleSceneIntro*		scene_intro = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	ModuleGUI*				GUI = nullptr;
	//Module3DImport*			Import_3D = nullptr;

	bool debug;
	bool renderPrimitives;

public:

	Timer	ms_timer;
	float	dt;
	float	fps;
	float	ms_limit;

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