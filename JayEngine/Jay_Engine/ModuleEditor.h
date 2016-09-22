#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"

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

private:
	bool showImGuiDemo = false;
	bool showGrid = true;

};

#endif // !__MODULEEDITOR_H__