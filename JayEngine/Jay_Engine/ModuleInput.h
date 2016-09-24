#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool startEnabled = true);
	~ModuleInput();

	bool init();
	update_status preUpdate(float dt);
	bool cleanUp();

	KEY_STATE getKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE getMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int getMouseX() const
	{
		return mouseX;
	}

	int getMouseY() const
	{
		return mouseY;
	}

	int getMouseXMotion() const
	{
		return mouseXMotion;
	}

	int getMouseYMotion() const
	{
		return mouseYMotion;
	}

	int getWheelYMotion()const
	{
		return wheelY;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouseX;
	int mouseY;
	int mouseXMotion;
	int mouseYMotion;
	//int mouseZMotion;
	int wheelY;
};

#endif // !__MODULEINPUT_H__