#pragma once

#include "imGUI\imgui.h"

class Application;

class UI_Comp
{
public:
	UI_Comp(Application* app);
	~UI_Comp();

	virtual void draw();
	bool isActive() { return active; }

public:
	bool active = false;

protected:
	Application* app = NULL;
};

