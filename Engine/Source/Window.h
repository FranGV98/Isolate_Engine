#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class Window : public Module
{
public:

	Window(bool start_enabled = true);

	// Destructor
	virtual ~Window();

	bool Init();
	bool CleanUp();
	int Width() const;
	int Height() const;

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__