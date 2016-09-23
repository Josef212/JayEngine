#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Globals.h"
#include "Timer.h"

#include <string>
#include <list>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class ModuleEditor;

//#include "MathGeoLib\include\MathGeoLibFwd.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleEditor* editor;

	bool debug = false;
	bool quit = false;

private:
	Timer	msTimer;
	Timer	fpsTimer;
	Uint32	frames = 0;
	float	dt;
	int		fpsCounter = 0;
	int		lastFrameMs = -1;
	int		lastFps = -1;
	int		cappedMs = 0;

	std::list<Module*> modules;
	std::string			title;
	std::string			organitzation;
	std::string			logs;

public:

	Application();
	~Application();

	bool init();
	update_status update();
	bool cleanUp();

	const char* getOrganitzation();
	const char* getTitle();
	void setOrganitzation(const char* org);
	void setTitle(const char* titl);

	uint getMaxFPS();
	void setMaxFPS(int maxFPS);

	void log(const char* str);

private:
	void addModule(Module* mod);
	void prepareUpdate();
	void finishUpdate();
};

//extern Application* app;

#endif // !__APPLICATION_H_