#include "Application.h"

#include "HrdInfo.h"
#include "Module.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
//#include "ModulePhysics3D.h"
#include "ModuleEditor.h"

Application::Application()
{
	info = new HrdInfo();
	LOG("Application Constructor --------------");
	fs = new ModuleFileSystem();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	camera = new ModuleCamera3D();
	renderer3D = new ModuleRenderer3D();
	//physics = new ModulePhysics3D();
	editor = new ModuleEditor();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	addModule(fs);
	addModule(window);
	addModule(camera);
	addModule(input);
	addModule(audio);
	//addModule(physics);
	addModule(editor);
	
	// Scenes


	// Renderer last!
	addModule(renderer3D);
}

Application::~Application()
{
	LOG("Application Destroctor --------------");
	for(std::list<Module*>::reverse_iterator it = modules.rbegin(); it!=modules.rbegin(); ++it)
	{
		RELEASE(*it);
	}
}

bool Application::init()
{
	bool ret = true;

	organitzation.assign("Josef21296");
	title.assign(TITLE);

	// Call Init() in all modules
	std::list<Module*>::iterator it = modules.begin();
	LOG("Application Init --------------");
	for(; it != modules.end() && ret == true; ++it)
	{
		ret = (*it)->init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	it = modules.begin();

	for (; it != modules.end() && ret == true; ++it)
	{
		if ((*it)->isEnabled())
			ret = (*it)->start();
	}

	info->setInfo();

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

	LOG("Application CleaUp --------------");
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

void Application::log(const char* str)
{
	logs.append(str);
	editor->log(str);
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