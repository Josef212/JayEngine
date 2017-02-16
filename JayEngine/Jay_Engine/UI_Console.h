#ifndef __UI_CCONSOLE__
#define __UI_CCONSOLE__

#include "UI_Panel.h"
#include <vector>
#include <string>

enum logType;

class UI_Console : public UI_Panel
{
public:
	UI_Console(bool startEnalbed = false);
	~UI_Console();

	void Draw()override;
	void LogUi(const char* str, logType type);

private:
	ImGuiTextBuffer logs;
	bool scrollDown = false;
	ImColor lastLogColor;
	std::vector<ImColor> colors;
	std::vector<std::string> log;
};


#endif //__UI_CCONSOLE__