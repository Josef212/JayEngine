#ifndef __MODULEMANAGER_H__
#define __MODULEMANAGER_H__

#include "Module.h"

class GameObject;
class Component;
struct aiScene;
struct aiNode;
struct aiMesh;

class JQuadTree;
class JOctree;

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
	Component* addCamera();

	GameObject* getSelected()const;
	void select(GameObject* toSelect);

	GameObject* loadFBX(char* file, char* path);
	GameObject* loadObjects(aiNode* node, const aiScene* scene, GameObject* parent);


	bool deleteGameObject(GameObject* toDel);

	GameObject* loadCube();

	void drawDebug();

	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

	/*void insertGameObjectToTree(GameObject* obj);
	void eraseGameObjectFromTree(GameObject* obj);*/

private:

public:
	int nextGOId = 0;
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;
	bool showTree = false;

	//JQuadTree* sceneTree = NULL;
	//JOctree* sceneTree = NULL;

private:

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;

	uint indexGO = 0;
};

#endif // !__MODULEMANAGER_H__