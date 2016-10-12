#ifndef __MODULEMANAGER_H__
#define __MODULEMANAGER_H__

#include "Module.h"

class ModuleManager : public Module
{
public:
	ModuleManager(bool startEnabled = true);
	virtual ~ModuleManager();

private:

public:

private:
	//Components limits, if 0 there is no limit. Should load that from json
	uint transformLimit = 1;
	uint meshLimit = 0; 
	uint materialLimit = 0;
};

#endif // !__MODULEMANAGER_H__