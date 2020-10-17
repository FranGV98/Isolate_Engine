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
#include "Module3DImport.h"

#include <list>
#include <vector>
#include <memory>

class Application
{
public:
	ModuleWindow*			window;
	ModuleInput*			input;
	ModuleSceneIntro*		scene_intro;
	ModuleRenderer3D*		renderer3D;
	ModuleCamera3D*			camera;
	ModuleGUI*				GUI;
	Module3DImport*			Import_3D;

	bool debug;
	bool renderPrimitives;
private:

	Timer	ms_timer;
	float	dt;

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