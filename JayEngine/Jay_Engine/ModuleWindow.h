#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool startEnabled = true);
	virtual ~ModuleWindow();

	bool init();
	bool cleanUp();

	void setTitle(const char* title);

	SDL_Window* getWindow() { return window; }

private:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;
};

#endif // __ModuleWindow_H__