#ifndef __MODULE_EVENT_SYSTEM__
#define __MODULE_EVENT_SYSTEM__


#include "Module.h"
#include "Event.h"

class ModuleEventSystem :
	public Module
{
public:
	ModuleEventSystem();
	virtual ~ModuleEventSystem();

	void SendGlobalEvent(const Event& e);
};

#endif // !__MODULE_EVENT_SYSTEM__