#include "Globals.h"
#include "Application.h"
#include "ModuleEventSystem.h"



ModuleEventSystem::ModuleEventSystem()
{
}


ModuleEventSystem::~ModuleEventSystem()
{
}

void ModuleEventSystem::sendGlobalEvent(const Event& e)
{
	app->sendGlobalEvent(e);
}