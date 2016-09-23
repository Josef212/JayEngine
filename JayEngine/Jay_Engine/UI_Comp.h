#pragma once

#include "imGUI\imgui.h"

class UI_Comp
{
public:
	UI_Comp();
	~UI_Comp();

	virtual void draw();
	bool isActive() { return active; }

protected:
	bool active;
};

