#include "Application.h"

#include "HrdInfo.h"
#include "RandGen.h"
#include "Module.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
//#include "ModulePhysics3D.h"
#include "ModuleEditor.h"
#include "ModuleResourceManager.h"
#include "ModuleGOManager.h"

#include "FileParser.h"

#include "SceneTry.h"

//TMP
#include "Time.h"

Application::Application()
{
	_LOG(LOG_STD, "Application Constructor --------------");

	time = new Time();

	info = new HrdInfo();
	random = new RandGen();

	fs = new ModuleFileSystem();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	camera = new ModuleCamera3D();
	renderer3D = new ModuleRenderer3D();
	//physics = new ModulePhysics3D();
	editor = new ModuleEditor();
	resourceManager = new ModuleResourceManager();
	goManager = new ModuleGOManager();

	sceneTry = new SceneTry();

	// Main Modules
	AddModule(editor);
	AddModule(fs);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	//AddModule(physics);
	AddModule(goManager);
	AddModule(resourceManager);
	
	// Scenes
	AddModule(sceneTry);

	// Renderer last
	AddModule(renderer3D);
}

Application::~Application()
{
	_LOG(LOG_STD, "Application Destructor --------------");
	for(std::list<Module*>::reverse_iterator it = modules.rbegin(); it!=modules.rend(); ++it)
	{
		RELEASE(*it);
	}

	RELEASE(random);
	RELEASE(time);
	RELEASE(info);
}

bool Application::Init()
{
	bool ret = true;

	char* buffer = nullptr;
	uint size = fs->Load("config.json", SETTINGS_PATH, &buffer);

	FileParser conf(buffer);

	ReadConfig(&conf.GetSection("app"));

	//NOTE/TODO: For now will pass Config on init but maybe will pass on start too

	// Call Init() in all modules
	std::list<Module*>::iterator it = modules.begin();
	_LOG(LOG_STD, "Application Init --------------");
	for(; it != modules.end() && ret == true; ++it)
	{
		ret = (*it)->Init(&conf.GetSection((*it)->name.c_str()));
	}

	// After all Init calls we call Start() in all modules
	_LOG(LOG_STD, "Application Start --------------");
	it = modules.begin();

	for (; it != modules.end() && ret == true; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Start();
	}

	info->SetInfo();

	RELEASE_ARRAY(buffer);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	time->UpdateTime();

	if (IsPlaySate())
		time->UpdateGameTime();

	dt = (float)msTimer.ReadSec();
	msTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (saveNextFrame)
		SaveGameNow();

	if (loadNextFrame)
		LoadGameNow();

	++frames;
	++fpsCounter;

	if (fpsTimer.Read() >= 1000)
	{
		lastFps = fpsCounter;
		fpsCounter = 0;
		fpsTimer.Start();
	}

	lastFrameMs = msTimer.Read();

	// cap fps
	if (cappedMs > 0 && (lastFrameMs < cappedMs))
		SDL_Delay(cappedMs - lastFrameMs);

	if (editor)
		editor->LogFPS((float)lastFps, (float)lastFrameMs);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	PrepareUpdate();
	
	std::list<Module*>::iterator it = modules.begin();
	
	for(; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->PreUpdate(dt);
	}

	it = modules.begin();

	for (; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Update(dt);
	}

	it = modules.begin();

	for (; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();

	if (quit)
		ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	_LOG(LOG_STD, "Application CleaUp --------------");
	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

void Application::DrawDebug()
{
	std::list<Module*>::iterator it = modules.begin();
	for (; it != modules.end(); ++it)
		if ((*it)->IsEnabled())
			(*it)->DrawDebug();
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}

const char* Application::GetOrganitzation()
{
	return organitzation.c_str();
}

const char* Application::GetTitle()
{
	return title.c_str();
}

void Application::SetOrganitzation(const char* org)
{
	if(org && org != organitzation)
		organitzation.assign(org);
	//TODO: filesystem should ajust
}

void Application::SetTitle(const char* titl)
{
	if (titl && titl != title)
	{
		title.assign(titl);
		window->SetTitle(titl);
		//TODO: filesystem should ajust
	}
}

void Application::Log(const char* str, logType type)
{
	logs.append(str);
	editor->Log(str, type);
}

uint Application::GetMaxFPS()
{
	if (cappedMs > 0)
		return (uint)((1.0f / (float)cappedMs) * 1000.0f);
	else
		return 0;
}

void Application::SetMaxFPS(int maxFPS)
{
	if (maxFPS > 0)
		cappedMs = 1000 / maxFPS;
	else
		cappedMs = 0;
}

void Application::Browse(const char * url) const
{
	ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}

void Application::ReadConfig(FileParser* conf)
{
	SetOrganitzation(conf->GetString("organitzation", "Josef21296"));
	SetTitle(conf->GetString("app_name", "JayEngine"));
	SetMaxFPS(conf->GetInt("fps_limit", 0));
	state = EDITOR; //TODO: check in config
}

void Application::SaveGame()
{
	saveNextFrame = true;
}

void Application::LoadGame()
{
	loadNextFrame = true;
}

bool Application::SaveGameNow() //TODO: pass the file name or set it from a string in .h in order to select it from the app
{
	bool ret = false;

	_LOG(LOG_INFO_REM, "SAVING!!!");

	FileParser conf;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		(*it)->Save(&conf.AddSection((*it)->name.c_str()));
	
	char* buffer;
	uint size = conf.WriteJson(&buffer, false);
	if (app->fs->Save("Data/Settings/c.json", buffer, size) != size)
		_LOG(LOG_ERROR, "Could not save json.");

	RELEASE_ARRAY(buffer);

	saveNextFrame = false;

	return ret;
}

bool Application::LoadGameNow()
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = fs->Load("c.json", SETTINGS_PATH, &buffer);

	FileParser conf(buffer);

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		(*it)->Load(&conf.GetSection((*it)->name.c_str()));
	
	_LOG(LOG_INFO_REM, "LOADING!!!");
	loadNextFrame = false;

	return ret;
}

gameState Application::GetGameState()const
{
	return state;
}

bool Application::IsEditorState()
{
	return state == EDITOR;
}

bool Application::IsPlaySate()
{
	return state == PLAY;
}

bool Application::IsPauseState()
{
	return state == PAUSE;
}

void Application::SendGlobalEvent(const Event& e)
{
	OnGlobalEvent(e);

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		if ((*it)->IsEnabled())
			(*it)->OnGlobalEvent(e);
	}
}

void Application::SetPlay()
{
	Event ev(Event::eventType::PLAY);
	SendGlobalEvent(ev);
	state = PLAY;
}

void Application::SetPause()
{
	Event ev(Event::eventType::PAUSE);
	SendGlobalEvent(ev);
	state = PAUSE;
}

void Application::SetStop()
{
	Event ev(Event::eventType::STOP);
	SendGlobalEvent(ev);
	state = EDITOR;
}

void Application::OnGlobalEvent(const Event& e)
{
	switch (e.type)
	{
		case Event::eventType::PLAY:
			if (state == PAUSE)				//Can do this ifs because we change the game state after this 
				time->StartGameTimer();
			else if(state == EDITOR)
				time->RestartGameTimer();
		break;

		case Event::eventType::PAUSE:
			if(state == PLAY)
				time->StopGameTimer();

		break;

		case Event::eventType::STOP:
			time->CleanGameTimer();
		break;
	}
}