#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool startEnabled = true);
	virtual ~ModuleWindow();

	bool init(FileParser* conf);
	bool cleanUp();

	void setTitle(const char* title);

	SDL_Window* getWindow() { return window; }	

	void hideWindow();
	void maximizeWindow();
	void minimizaWindow();
	void restoreWindow();
	void showWindow();
	void raiseWindow();

	bool isScreecnSaver();
	void setScreenSaver(bool set);

	void setFullScreen(bool set);
	bool isFullScreen();

	void setResizable(bool set);
	bool isResizable() const;

	bool isBorderless() const;
	void setBorderless(bool set);

	bool isFullscreenDesktop() const;
	void setFullScreenDesktop(bool set);

	void setBorder(bool set);

	void setGrab(bool set);

	void setPosition(int x, int y);
	void getPosition(int& w, int& h);

	void setSize(int w, int h);
	void getWindowSize(int& width, int& height) const;
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	
	bool setBrightness(float bright);
	float getBrightness();
	
	void getRange(int& minW, int& minH, int& maxW, int& maxH);
	int getRefresh();

	void setIcon(SDL_Surface* icon);
	//void setIcon(const char* file);

private:
	int width;
	int height;
	bool fullscreen, resizable, borderless, fullscreenDesktop;//TODO: save and load changes on this variables

	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;
};

#endif // __ModuleWindow_H__