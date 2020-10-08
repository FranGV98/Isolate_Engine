#ifndef __MODULE_GUI_H__
#define __MODULE_GUI_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "imgui/imgui.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = true);
	~ModuleGUI();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	ImGuiIO io;
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
	bool fullscreen;
};

#endif // !__MODULE_GUI_H__