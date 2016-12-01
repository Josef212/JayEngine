#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include <string>
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

	GameObject* loadPrefab(const char* file, const char* path = NULL); //Must be a json wich contains all .jof and .dds, etc


	bool deleteGameObject(GameObject* toDel);
	void cleanRoot();

	GameObject* loadCube();

	void drawDebug();

	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

	void insertGameObjectToTree(GameObject* obj);
	void eraseGameObjectFromTree(GameObject* obj); //DEL_COM

	bool saveScene(const char* name, const char* path = NULL);
	bool loadScene(const char* name, const char* path = NULL);

	bool setCurrentScene(const char* scene);
	const char* getCurrentScene();

	void onGlobalEvent(const Event& e);

private:
	GameObject* recFindGO(UID id, GameObject* go);
	void loadSceneOrPrefabs(FileParser& file);

	void onPlay();
	void onPause();
	void onStop();

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

	std::string currentScene;
	//TODO: List/Map of all scenes?? Should a scene be a resource and the list/map be managed as a resource?
	/**
		For now as a scene is basically a root game object, will leave it here but must check this in the future.
	*/
};

#endif // !__MODULEGOMANAGER_H__