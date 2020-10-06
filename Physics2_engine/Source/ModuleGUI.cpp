#include "Globals.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
}
ModuleGUI::~ModuleGUI()
{}

bool ModuleGUI::Init()
{
	LOG("Creating GUI context");
	bool ret = true;

	return ret;
};

update_status ModuleGUI::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	return true;
}