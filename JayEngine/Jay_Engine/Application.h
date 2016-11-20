#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Time.h"
#include "Globals.h"
#include "Timer.h"
#include "Event.h"

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
class ModuleResourceManager;
class ModuleGOManager;

class SceneTry;

class HrdInfo;
class RandGen;
class FileParser;

enum gameState
{
	EDITOR = 0,
	PLAY,
	PAUSE
};

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
	ModuleResourceManager* resourceManager = NULL;
	ModuleGOManager* goManager = NULL;

	SceneTry* sceneTry = NULL;

	HrdInfo* info = NULL;
	RandGen* random = NULL;

	bool debug = false;
	bool quit = false;

	gameState state = EDITOR;

private:
	Timer	msTimer;
	Timer	fpsTimer;
	Uint32	frames = 0;
	float	dt;
	int		fpsCounter = 0;
	Uint32		lastFrameMs = -1;
	int		lastFps = -1;
	Uint32		cappedMs = 0;

	std::list<Module*> modules;
	std::string			title;
	std::string			organitzation;
	std::string			logs;

	bool saveNextFrame = false;
	bool loadNextFrame = false;

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

	void saveGame();//TODO: saves files etc
	void loadGame();

	gameState getGameState()const;
	bool isEditorState();
	bool isPlaySate();
	bool isPauseState();

	void setPlay();
	void setPause();
	void setStop();

	void sendGlobalEvent(const Event& e);
	void onGlobalEvent(const Event& e);

private:
	void addModule(Module* mod);
	void prepareUpdate();
	void finishUpdate();

	void readConfig(FileParser* conf);

	bool saveGameNow(); //Basically for config, if in any module needs to save a json just save it there
	bool loadGameNow(); //same as in save but loading
};

extern Application* app;

#endif // !__APPLICATION_H_