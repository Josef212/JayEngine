#pragma once

#include "Module.h"

class SceneTry : public Module
{
public:
	SceneTry(bool startEnabled = true);
	~SceneTry();

	bool init();
	bool start();
	update_status preUpdate(float dt);
	update_status udate(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	//TMP
	/*uint vArray = 0;
	uint verticesId = 0, indicesId = 0;*/
	uint texId = 0;
};

