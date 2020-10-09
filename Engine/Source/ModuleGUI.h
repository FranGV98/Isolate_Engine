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
	void RequestBrowser(const char*);

public:
	ImGuiIO io;
	bool show_demo_window = false;
	bool show_about_window = false;
	bool show_config_window = false;
	ImVec4 clear_color;

	char label[25];
	char title[25];

	char values[25];
	char format[10];
	int bright_int, width_int, height_int;
	std::vector<float> fps_log, ms_log;
	Timer frame_time;

	bool fullscreen = false;
	
};

#endif // !__MODULE_GUI_H__