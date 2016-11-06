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
class ModuleFileSystem;
class ModuleImporter;
class ModuleManager;

class SceneTry;

class HrdInfo;
class RandGen;
class FileParser;

class Application
{
public:
	ModuleFileSystem* fs = NULL;
	ModuleWindow* window = NULL;
	ModuleInput* input = NULL;
	ModuleAudio* audio = NULL;
	ModuleRenderer3D* renderer3D = NULL;
	ModuleCamera3D* camera = NULL;
	ModulePhysics3D* physics = NULL;
	ModuleEditor* editor = NULL;
	ModuleImporter* importer = NULL;
	ModuleManager* manager = NULL;

	SceneTry* sceneTry = NULL;

	HrdInfo* info = NULL;
	RandGen* random = NULL;

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

	void drawDebug();

	const char* getOrganitzation();
	const char* getTitle();
	void setOrganitzation(const char* org);
	void setTitle(const char* titl);

	uint getMaxFPS();
	void setMaxFPS(int maxFPS);

	void log(const char* str, logType type);
	void browse(const char* url) const;

private:
	void addModule(Module* mod);
	void prepareUpdate();
	void finishUpdate();

	void readConfig(FileParser* conf);
};

extern Application* app;

#endif // !__APPLICATION_H_