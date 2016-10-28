#ifndef __MODULEMANAGER_H__
#define __MODULEMANAGER_H__

#include "Module.h"

class GameObject;
class Component;
struct aiScene;
struct aiNode;
struct aiMesh;

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

	void draw();

	GameObject* getSceneroot()const;
	GameObject* createEmptyGO();
	GameObject* createCamera();

	Component* addTransform();
	Component* addMesh();
	Component* addMaterial();

	GameObject* getSelected()const;
	void select(GameObject* toSelect);

	GameObject* loadFBX(char* file, char* path);
	GameObject* loadObjects(aiNode* node, const aiScene* scene, GameObject* parent);

	bool deleteGameObject(GameObject* toDel);

	GameObject* loadCube();

	void drawDebug();

	void makeGOShowAABox(bool show);
	void makeGOShowOBox(bool show);

private:
	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

public:
	int nextGOId = 0;
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;

private:
	//Components limits, if 0 there is no limit. Should load that from json
	uint transformLimit = 1;
	uint meshLimit = 0; 
	uint materialLimit = 0;

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;

	uint indexGO = 0;
};

#endif // !__MODULEMANAGER_H__