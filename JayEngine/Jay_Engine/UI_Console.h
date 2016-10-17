#ifndef __UICCONSOLE_H__
#define __UICCONSOLE_H__

#include "UI_Comp.h"
#include <vector>
#include <string>

enum logType;

class UI_Console : public UI_Comp
{
public:
	UI_Console();
	~UI_Console();

	void draw();
	void logUi(const char* str, logType type);

private:
	ImGuiTextBuffer logs;
	bool scrollDown = false;
	ImColor lastLogColor;
	std::vector<ImColor> colors;
	std::vector<std::string> log;
};


#endif //__UICCONSOLE_H__