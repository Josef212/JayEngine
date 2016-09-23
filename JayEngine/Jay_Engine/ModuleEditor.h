#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include <list>

class UI_Comp;
class UI_Conf;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool startEnabled = true);
	~ModuleEditor();

	bool init();
	bool start();
	update_status preUpdate(float dt);
	update_status update(float dt);
	bool cleanUp();

	void drawEditor();
	void passInput(SDL_Event* inputEvent);

	void logFPS(float fps, float ms);

public:
	std::list<UI_Comp*> uiList;

	UI_Conf* conf = NULL;

private:
	bool showImGuiDemo = false;
	bool showGrid = true;
	bool showConfig = false;

};

#endif // !__MODULEEDITOR_H__