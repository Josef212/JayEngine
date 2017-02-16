#include "Globals.h"
#include "Application.h"
#include "ModuleEventSystem.h"



ModuleEventSystem::ModuleEventSystem()
{
}


ModuleEventSystem::~ModuleEventSystem()
{
}

void ModuleEventSystem::SendGlobalEvent(const Event& e)
{
	app->SendGlobalEvent(e);
}