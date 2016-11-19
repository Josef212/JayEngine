#ifndef __MODULEEVENTSYSTEM_H__
#define __MODULEEVENTSYSTEM_H__


#include "Module.h"
#include "Event.h"

class ModuleEventSystem :
	public Module
{
public:
	ModuleEventSystem();
	virtual ~ModuleEventSystem();

	void sendGlobalEvent(const Event& e);
};

#endif // !__MODULEEVENTSYSTEM_H__