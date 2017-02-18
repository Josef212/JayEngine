#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Input: Creation.");
	
	name.assign("module_input");

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	_LOG(LOG_STD, "Input: Destroying.");
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init(FileParser* conf)
{
	_LOG(LOG_STD, "Input: Init.");
	_LOG(LOG_INFO, "Init SDL input event system");
	bool ret = true;

	SDL_Init(0);

	//SDL_ShowCursor(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		_LOG(LOG_ERROR, "SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= app->window->GetWinSize();
	mouseY /= app->window->GetWinSize();
	wheelY = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_IDLE)
				mouseButtons[i] = KEY_DOWN;
			else
				mouseButtons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_REPEAT || mouseButtons[i] == KEY_DOWN)
				mouseButtons[i] = KEY_UP;
			else
				mouseButtons[i] = KEY_IDLE;
		}
	}

	mouseXMotion = mouseYMotion = 0;

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		app->editor->PassInput(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
				wheelY = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouseX = e.motion.x / app->window->GetWinSize();
			mouseY = e.motion.y / app->window->GetWinSize();

			mouseXMotion = e.motion.xrel / app->window->GetWinSize();
			mouseYMotion = e.motion.yrel / app->window->GetWinSize();
			break;

			case SDL_QUIT:
				app->quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				switch (e.window.type)
				{
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						Event ev(Event::WIN_RESIZE);
						ev.point2d.x = e.window.data1;
						ev.point2d.y = e.window.data2;
						app->SendGlobalEvent(ev);
				}
			}
			break;
		}
	}

	SDL_PollEvent(&e);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	_LOG(LOG_STD, "Input: CleanUp.");
	_LOG(LOG_INFO, "Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}