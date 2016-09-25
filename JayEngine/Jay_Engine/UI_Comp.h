#ifndef __UICOMP_H__
#define __UICOMP_H__

#include "imGUI\imgui.h"

class Application;

class UI_Comp
{
public:
	UI_Comp();
	~UI_Comp();

	virtual void draw();
	bool isActive() { return active; }
	void swapActive();

public:
	bool active = false;

protected:
};


#endif //__UICOMP_H__