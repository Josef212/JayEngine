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
#include "ModuleImporter.h"
#include "ModuleManager.h"

#include "FileParser.h"

#include "SceneTry.h"

//TMP
#include "jsoncpp\json\json.h"

Application::Application()
{
	_LOG(LOG_STD, "Application Constructor --------------");

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
	importer = new ModuleImporter();
	manager = new ModuleManager();

	sceneTry = new SceneTry();

	// Main Modules
	addModule(fs);
	addModule(window);
	addModule(camera);
	addModule(input);
	addModule(audio);
	//addModule(physics);
	addModule(editor);
	addModule(manager);
	addModule(importer);
	
	// Scenes
	addModule(sceneTry);

	// Renderer last
	addModule(renderer3D);
}

Application::~Application()
{
	_LOG(LOG_STD, "Application Destructor --------------");
	for(std::list<Module*>::reverse_iterator it = modules.rbegin(); it!=modules.rbegin(); ++it)
	{
		RELEASE(*it);
	}
}

bool Application::init()
{
	bool ret = true;

	char* buffer = NULL;
	uint size = fs->load("config.json", SETTINGS_PATH, &buffer);

	FileParser conf(buffer);

	readConfig(&conf.getSection("app"));

	//NOTE/TODO: For now will pass Config on init but maybe will pass on start too

	// Call Init() in all modules
	std::list<Module*>::iterator it = modules.begin();
	_LOG(LOG_STD, "Application Init --------------");
	for(; it != modules.end() && ret == true; ++it)
	{
		ret = (*it)->init(&conf.getSection((*it)->name.c_str()));
	}

	// After all Init calls we call Start() in all modules
	_LOG(LOG_STD, "Application Start --------------");
	it = modules.begin();

	for (; it != modules.end() && ret == true; ++it)
	{
		if ((*it)->isEnabled())
			ret = (*it)->start();
	}

	info->setInfo();

	RELEASE_ARRAY(buffer);

	return ret;
}

// ---------------------------------------------
void Application::prepareUpdate()
{
	dt = (float)msTimer.ReadSec();
	msTimer.Start();
}

// ---------------------------------------------
void Application::finishUpdate()
{
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

	if (cappedMs > 0 && lastFrameMs < cappedMs)
	{
		SDL_Delay(cappedMs - lastFrameMs);
	}

	if (editor)
		editor->logFPS((float)lastFps, (float)lastFrameMs);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::update()
{
	update_status ret = UPDATE_CONTINUE;

	if (input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	prepareUpdate();
	
	std::list<Module*>::iterator it = modules.begin();
	
	for(; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->isEnabled())
			ret = (*it)->preUpdate(dt);
	}

	it = modules.begin();

	for (; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->isEnabled())
			ret = (*it)->update(dt);
	}

	it = modules.begin();

	for (; it != modules.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if ((*it)->isEnabled())
			ret = (*it)->postUpdate(dt);
	}

	finishUpdate();

	if (quit)
		ret = UPDATE_STOP;

	return ret;
}

bool Application::cleanUp()
{
	bool ret = true;

	_LOG(LOG_STD, "Application CleaUp --------------");
	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rbegin(); ++it)
	{
		ret = (*it)->cleanUp();
	}

	return ret;
}

void Application::drawDebug()
{
	std::list<Module*>::iterator it = modules.begin();
	for (; it != modules.end(); ++it)
		if ((*it)->isEnabled())
			(*it)->drawDebug();
}

void Application::addModule(Module* mod)
{
	modules.push_back(mod);
}

const char* Application::getOrganitzation()
{
	return organitzation.c_str();
}

const char* Application::getTitle()
{
	return title.c_str();
}

void Application::setOrganitzation(const char* org)
{
	if(org && org != organitzation)
		organitzation.assign(org);
	//TODO: filesystem should ajust
}

void Application::setTitle(const char* titl)
{
	if (titl && titl != title)
	{
		title.assign(titl);
		window->setTitle(titl);
		//TODO: filesystem should ajust
	}
}

void Application::log(const char* str, logType type)
{
	logs.append(str);
	editor->log(str, type);
}

uint Application::getMaxFPS()
{
	if (cappedMs > 0)
		return (uint)((1.0f / (float)cappedMs) * 1000.0f);
	else
		return 0;
}

void Application::setMaxFPS(int maxFPS)
{
	if (maxFPS > 0)
		cappedMs = 1000 / maxFPS;
	else
		cappedMs = 0;
}

void Application::browse(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::readConfig(FileParser* conf)
{
	std::string str = conf->getStdString("organitzation", "Josef21296");
	organitzation.assign(str);
	str.assign(conf->getStdString("app_name", "Jay_Engine"));
	setTitle(str.c_str());
}