#pragma once

#include "Module.h"

class GameObject;

class SceneTry : public Module
{
public:
	SceneTry(bool startEnabled = true);
	~SceneTry();

	bool Start()override;
	update_status PreUpdate(float dt)override;
	update_status Update(float dt)override;
	update_status PostUpdate(float dt)override;
	bool CleanUp()override;

	//TMP
	/*uint vArray = 0;
	uint verticesId = 0, indicesId = 0;*/
	//uint texId = 0;
};

