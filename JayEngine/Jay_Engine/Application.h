#ifndef __APPLICATION__
#define __APPLICATION__

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
	ModuleFileSystem* fs = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModulePhysics3D* physics = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleResourceManager* resourceManager = nullptr;
	ModuleGOManager* goManager = nullptr;

	SceneTry* sceneTry = nullptr;

	HrdInfo* info = nullptr;
	RandGen* random = nullptr;

	bool debug = false;
	bool quit = false;
	bool forceEditor = false;

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

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawDebug();

	const char* GetOrganitzation();
	const char* GetTitle();
	void SetOrganitzation(const char* org);
	void SetTitle(const char* titl);

	uint GetMaxFPS();
	void SetMaxFPS(int maxFPS);

	void Log(const char* str, logType type);
	void Browse(const char* url) const;

	void SaveGame();//TODO: saves files etc
	void LoadGame();

	gameState GetGameState()const;
	bool IsEditorState();
	bool IsPlaySate();
	bool IsPauseState();

	void SetPlay();
	void SetPause();
	void SetStop();

	void SendGlobalEvent(const Event& e);
	void OnGlobalEvent(const Event& e);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void ReadConfig(FileParser* conf);

	bool SaveGameNow(); //Basically for config, if in any module needs to save a json just save it there
	bool LoadGameNow(); //same as in save but loading
};

extern Application* app;

#endif // !__APPLICATION__