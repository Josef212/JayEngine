#ifndef __MODULE_WINDOW__
#define __MODULE_WINDOW__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool startEnabled = true);
	virtual ~ModuleWindow();

	bool Init(FileParser* conf)override;
	bool CleanUp()override;

	void SetTitle(const char* title);

	SDL_Window* GetWindow() { return window; }	

	void HideWindow();
	void MaximizeWindow();
	void MinimizaWindow();
	void RestoreWindow();
	void ShowWindow();
	void RaiseWindow();

	bool IsScreenSaver()const;
	void SetScreenSaver(bool set);

	void SetFullScreen(bool set);
	bool IsFullScreen()const;

	void SetResizable(bool set);
	bool IsResizable() const;

	bool IsBorderless() const;
	void SetBorderless(bool set);

	bool IsFullscreenDesktop() const;
	void SetFullScreenDesktop(bool set);

	void SetBorder(bool set);

	void SetGrab(bool set);

	void SetPosition(int x, int y);
	void GetPosition(int& w, int& h)const;

	void SetSize(int w, int h);
	void GetWindowSize(int& width, int& height) const;
	void SetWidth(int width);
	int GetWidth()const;
	void SetHeight(int height);
	int GetHeight()const;

	int GetWinSize()const;
	
	bool SetBrightness(float bright);
	float GetBrightness()const;
	
	void GetRange(int& minW, int& minH, int& maxW, int& maxH)const;
	int GetRefresh()const;

	void OnGlobalEvent(const Event& e);

	void SetIcon(SDL_Surface* icon);
	//void setIcon(const char* file);

private:
	int winSize = 1;
	int width = 1;
	int height = 1;
	bool fullscreen, resizable, borderless, fullscreenDesktop;//TODO: save and load changes on this variables

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;
};

#endif // __MODULE_WINDOW__