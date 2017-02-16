#ifndef __UI_PANEL__
#define __UI_PANEL__

#include "imGUI\imgui.h"

class Application;

class UI_Panel
{
public:
	UI_Panel(bool startEnalbed = false) : active(startEnalbed)
	{}

	virtual ~UI_Panel()
	{}

	virtual void Draw()
	{}

	bool IsActive() { return active; }
	void SwapActive() { active = !active; }

public:
	bool active = false;

protected:
};


#endif //__UI_PANEL__