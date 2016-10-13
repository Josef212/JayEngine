#ifndef __MODULEMANAGER_H__
#define __MODULEMANAGER_H__

#include "Module.h"

class GameObject;
class Component;

class ModuleManager : public Module
{
public:
	ModuleManager(bool startEnabled = true);
	virtual ~ModuleManager();

	bool init();
	update_status preUpdate(float dt);
	update_status update(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	GameObject* getSceneroot()const;
	GameObject* createEmptyGO();

	Component* addTransform();
	Component* addMesh();
	Component* addMaterial();

	GameObject* getSelected()const;
	void select(GameObject* toSelect);

	GameObject* loadFBX(const char* file, const char* path);
private:

public:
	int nextGOId = 0;

private:
	//Components limits, if 0 there is no limit. Should load that from json
	uint transformLimit = 1;
	uint meshLimit = 0; 
	uint materialLimit = 0;

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;
};

#endif // !__MODULEMANAGER_H__