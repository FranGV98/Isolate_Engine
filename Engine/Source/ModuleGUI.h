#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

public:
	SDL_Window* window;
	SDL_WindowFlags window_flags;
	SDL_GLContext gl_context;
	bool show_demo_window;
};