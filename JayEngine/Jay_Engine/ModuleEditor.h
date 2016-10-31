#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include <list>

class UI_Comp;
class UI_Conf;
class UI_Console;
class UI_Hierarchy;
class UI_Inspector;

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool startEnabled = true);
	~ModuleEditor();

	bool init();
	bool start();
	update_status preUpdate(float dt);
	update_status update(float dt);
	bool cleanUp();

	void drawEditor();
	void passInput(SDL_Event* inputEvent);

	void logFPS(float fps, float ms);
	void log(const char* str, logType type);

private:

public:
	std::list<UI_Comp*> uiList;

	UI_Conf* conf = NULL;
	UI_Console* console = NULL;
	UI_Hierarchy* hieracy = NULL;
	UI_Inspector* inspector = NULL;

private:
	bool showImGuiDemo = false;
	bool showConfig = false;
	bool showAbout = false;

};

#endif // !__MODULEEDITOR_H__