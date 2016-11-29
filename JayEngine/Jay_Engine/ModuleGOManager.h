#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include <map>

class GameObject;
class Component;
struct aiScene;
struct aiNode;
struct aiMesh;

class JOctree;

class ModuleGOManager : public Module
{
public:
	ModuleGOManager(bool startEnabled = true);
	virtual ~ModuleGOManager();

	bool init(FileParser* conf);
	update_status preUpdate(float dt);
	update_status update(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	void draw();

	GameObject* getSceneroot()const;
	GameObject* getGameObjectFromId(UID id);

	GameObject* createEmptyGO();
	GameObject* createEmptyGoWithAABB(float xP, float yP, float zP); //TMP
	GameObject* createCamera();

	Component* addTransform();
	Component* addMesh();
	Component* addMaterial();
	Component* addCamera();

	GameObject* getSelected()const;
	void select(GameObject* toSelect);

	GameObject* loadFBX(char* file, char* path);
	GameObject* loadObjects(aiNode* node, const aiScene* scene, GameObject* parent);

	GameObject* loadPrefab(const char* file, const char* path = NULL); //Must be a json wich contains all .jof and .dds, etc


	bool deleteGameObject(GameObject* toDel);

	GameObject* loadCube();

	void drawDebug();

	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

	void insertGameObjectToTree(GameObject* obj);
	void eraseGameObjectFromTree(GameObject* obj); //DEL_COM

	void saveScene(const char* name);
	void loadScene(const char* name);

private:
	GameObject* recFindGO(UID id, GameObject* go);

public:
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;
	bool showTree = false;

	JOctree* sceneTree = NULL;

	//-------
	std::map<std::string, uint> texturesLoaded;  //textures paths and idBuffers

private:

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;

	uint indexGO = 0;
};

#endif // !__MODULEGOMANAGER_H__