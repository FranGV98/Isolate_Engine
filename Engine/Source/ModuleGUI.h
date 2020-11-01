#ifndef __MODULE_GUI_H__
#define __MODULE_GUI_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "imgui/imgui.h"
#include "Gameobject.h"

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

	void ConsoleLog(char* log);
	void ClearConsole();
	void ShortKeys();
	bool SetDocking(ImGuiWindowFlags window_flag);
	void ObtainGameObjects(GameObject* go);
	void SelectGameObject(GameObject* selected_object);

public:
	bool show_demo_window = false;
	bool show_about_window = false;
	bool show_config_window = false;
	bool show_console_window = false;
	bool show_inspector_window = false;
	bool show_hierarchy_window = false;

	bool docking_window = true;
	//bool p_open = true;

	ImVec4 clear_color;

	char label[25];
	char title[25];

	char values[25];
	char format[10];
	float bright_float = 1.0f;
	int  width_int, height_int;
	std::vector<float> fps_log, ms_log;
	int max_fps;
	Timer frame_time;

	bool fullscreen = false;
	bool wireframe = false;
	bool gl_depth = true;
	bool gl_cull_face = false;
	bool gl_light = true;
	bool gl_color_mat = true;
	bool draw_normals_dir = false;

	const char* resolution[5] = { "3840 x 2160","2560 x 1440","1920 x 1080", "1290 x 720", "720 x 480" };
	int current_resolution = 3;

	std::vector<char*> logs;
	bool update_scroll;
	
	GameObject* selected_gameobject = nullptr;
	GameObject* dragged_object = nullptr;
	//
	//float3 go_pos;
	//Quat go_rot;
	//float3 go_scale;
	//

	ImGuiTreeNodeFlags	tree_flags;

};

#endif // !__MODULE_GUI_H__