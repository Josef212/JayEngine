#ifndef __UICOMP_H__
#define __UICOMP_H__

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


#endif //__UICOMP_H__