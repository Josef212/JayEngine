#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "FileParser.h"

ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Window: Creation.");

	name.assign("module_window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
	_LOG(LOG_STD, "Window: Destroying.");
}

// Called before render is available
bool ModuleWindow::Init(FileParser* conf)
{
	_LOG(LOG_STD, "Window: Init.");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		_LOG(LOG_ERROR, "SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		winSize = conf->GetInt("screen_size", 1);
		width = conf->GetInt("width", 1920) * winSize;
		height = conf->GetInt("height", 1080) * winSize;

		fullscreen = conf->GetBool("fullscreen", false);
		resizable = conf->GetBool("resizable", true);
		borderless = conf->GetBool("borderless", false);
		fullscreenDesktop = conf->GetBool("fullscreen_desktop", false);

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED;

		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreenDesktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
		{
			_LOG(LOG_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	_LOG(LOG_STD, "Window: CleanUp.");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::HideWindow()
{
	if (window)
		SDL_HideWindow(window);
	else
		_LOG(LOG_ERROR, "Error hidding window, null pointer.");
}

void ModuleWindow::MaximizeWindow()
{
	if (window)
		SDL_MaximizeWindow(window);
	else
		_LOG(LOG_ERROR, "Error maximizing window, null pointer.");
}

void ModuleWindow::MinimizaWindow()
{
	if (window)
		SDL_MinimizeWindow(window);
	else
		_LOG(LOG_ERROR, "Error minimizing window, null pointer.");
}

void ModuleWindow::RestoreWindow()
{
	if (window)
		SDL_RestoreWindow(window);
	else
		_LOG(LOG_ERROR, "Error restoring window, null pointer.");
}

void ModuleWindow::ShowWindow()
{
	if (window)
		SDL_ShowWindow(window);
	else
		_LOG(LOG_ERROR, "Error showing window, null pointer.");
}

void ModuleWindow::RaiseWindow()
{
	if (window)
		SDL_RaiseWindow(window);
	else
		_LOG(LOG_ERROR, "Error raising window, null pointer.");
}

bool ModuleWindow::IsScreenSaver()const
{
	return (bool)SDL_IsScreenSaverEnabled();
}

void ModuleWindow::SetScreenSaver(bool set)
{
	if (set)
		SDL_EnableScreenSaver();
	else
		SDL_DisableScreenSaver();

	_LOG(LOG_STD, "Screen saver setted in to: %d.", (int)IsScreenSaver());
}

void ModuleWindow::SetFullScreen(bool set)
{
	if (window)
	{
		if (set != fullscreen)
		{
			fullscreen = set;
			if (fullscreen == true)
			{
				if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
					_LOG(LOG_ERROR, "Could not switch to fullscreen: %s\n", SDL_GetError());
				fullscreenDesktop = false;
			}
			else
			{
				if (SDL_SetWindowFullscreen(window, 0) != 0)
					_LOG(LOG_ERROR, "Could not switch to windowed: %s\n", SDL_GetError());
			}
		}
	}
	else
		_LOG(LOG_ERROR, "Error setting fullscreen, window pointer is nullptr");
}

bool ModuleWindow::IsFullScreen()const
{
	return fullscreen;
}

bool ModuleWindow::IsResizable() const
{
	return resizable;
}

void ModuleWindow::SetResizable(bool set)
{
	resizable = set; //cant change this, save it to load it on next exe
}

bool ModuleWindow::IsBorderless() const
{
	return borderless;
}

void ModuleWindow::SetBorderless(bool set)
{
	if (set != borderless && !fullscreen && !fullscreenDesktop)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	}
}

bool ModuleWindow::IsFullscreenDesktop() const
{
	return fullscreenDesktop;
}

void ModuleWindow::SetFullScreenDesktop(bool set)
{
	if (set != fullscreenDesktop)
	{
		fullscreenDesktop = set;
		if (fullscreenDesktop)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				_LOG(LOG_ERROR, "Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				_LOG(LOG_ERROR, "Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

void ModuleWindow::SetBorder(bool set)
{
	if (SDL_GetWindowFlags(window) != SDL_WINDOW_FULLSCREEN)
	{
		if (set)
			SDL_SetWindowBordered(window, SDL_TRUE);
		else
			SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
		_LOG(LOG_ERROR, "Could not set border, because of fullscreen.");

}

void ModuleWindow::SetGrab(bool set)
{
	if (window)
	{
		if (set)
			SDL_SetWindowGrab(window, SDL_TRUE);
		else
			SDL_SetWindowGrab(window, SDL_FALSE);
	}
	else
		_LOG(LOG_ERROR, "Error setting grab, window pointer is nullptr");

}

void ModuleWindow::SetPosition(int x, int y)
{
	if (window)
	{
		SDL_SetWindowPosition(window, x, y);
	}
	else
		_LOG(LOG_ERROR, "Error setting window position, window pointer is nullptr");
}

void ModuleWindow::GetPosition(int& w, int& h)const
{
	if (window)
	{
		SDL_GetWindowPosition(window, &w, &h);
	}
	else
		_LOG(LOG_ERROR, "Error getting window position, window pointer is nullptr.");
}

void ModuleWindow::SetSize(int w, int h)
{
	if (window && w >= 0 && h >= 0)
	{
		width = w;
		height = h;
		SDL_SetWindowSize(window, w, h);
	}
	else
		_LOG(LOG_ERROR, "Error setting window size, window pointer is nullptr");
}

void ModuleWindow::GetWindowSize(int& width, int& height)const
{
	width = this->width;
	height = this->height;
}

int ModuleWindow::GetWidth()const
{
	return width;
}

void ModuleWindow::SetWidth(int width)
{
	if (width > 0)
	{
		this->width = width;
		SetSize(width, height);
	}
}

void ModuleWindow::SetHeight(int height)
{
	if (height > 0)
	{
		this->height = height;
		SetSize(width, height);
	}
}

int ModuleWindow::GetHeight()const
{
	return height;
}

int ModuleWindow::GetWinSize()const
{
	return winSize;
}

bool ModuleWindow::SetBrightness(float bright)
{
	bool ret = true;

	if (bright < 0.0f)
		bright = 0.0f;
	if (bright > 1.0f)
		bright = 1.0f;
	if (window)
	{
		if (SDL_SetWindowBrightness(window, bright) < 0)
		{
			ret = false;
			_LOG(LOG_ERROR, "Error setting brightness, SDL_Error: %s.\n", SDL_GetError());
		}
	}
	else
		_LOG(LOG_ERROR, "Error setting brightness, window pointer is nullptr");

	return ret;
}

float ModuleWindow::GetBrightness()const
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::GetRange(int& minW, int& minH, int& maxW, int& maxH)const
{
	minW = 640;
	minH = 480;
	maxW = 3000;
	maxH = 2000;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		_LOG(LOG_ERROR, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	else
	{
		maxW = dm.w;
		maxH = dm.h;
	}
}

int ModuleWindow::GetRefresh()const
{
	uint ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		_LOG(LOG_ERROR, "Error getting refresh rate: %s", SDL_GetError());
	}
	else
		ret = dm.refresh_rate;

	return ret;
}

void ModuleWindow::SetIcon(SDL_Surface* icon)
{
	if (window)
	{
		SDL_SetWindowIcon(window, icon);
	}
	else
		_LOG(LOG_ERROR, "Error setting window icon, window pointer is nullptr.");
}

void ModuleWindow::OnGlobalEvent(const Event& e)
{
	//TODO
}

/*void ModuleWindow::SetIcon(const char* file) //TODO: can use it when textures are ready
{
	if (window)
	{
		SDL_Surface* surface = app->textures->loadSurface(file);
		SDL_SetWindowIcon(window, surface);
	}
	else
		_LOG(LOG_ERROR, "Error setting window icon, window pointer is nullptr.");
}*/