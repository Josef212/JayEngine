#ifndef __UICCONSOLE_H__
#define __UICCONSOLE_H__

#include "UI_Comp.h"

class UI_Console : public UI_Comp
{
public:
	UI_Console(Application* app);
	~UI_Console();

	void draw();
	void logUi(const char* str);

private:
	ImGuiTextBuffer logs;
};


#endif //__UICCONSOLE_H__