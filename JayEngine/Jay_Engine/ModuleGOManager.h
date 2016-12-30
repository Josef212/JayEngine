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
	bool start();
	update_status preUpdate(float dt);
	update_status update(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	void draw();

	void removeFlaggedGO();

	GameObject* getSceneroot()const;
	GameObject* getGameObjectFromId(UID id);


	GameObject* createGameObject(GameObject* parent = NULL);
	GameObject* createEmptyGO();
	GameObject* createEmptyGoWithAABB(float xP, float yP, float zP); //TMP
	GameObject* createCamera();

	Component* addTransform();
	Component* addMesh();
	Component* addMaterial();
	Component* addCamera();

	GameObject* getSelected()const;
	void select(GameObject* toSelect);

	GameObject* validateGO(const GameObject* point)const;

	GameObject* loadPrefab(const char* file, const char* path = NULL); //Must be a json wich contains all .jof and .dds, etc

	void cleanRoot();
	void cleanRootNow();

	GameObject* loadCube();

	void drawDebug();

	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

	void insertGameObjectToTree(GameObject* obj);
	void eraseGameObjectFromTree(GameObject* obj); //DEL_COM

	void saveScene();
	void loadScene();

	void loadSceneOrPrefabs(const FileParser& file);

	bool setCurrentScene(const char* scene);
	const char* getCurrentScene();

	void onGlobalEvent(const Event& e);

private:
	GameObject* recFindGO(UID id, GameObject* go);
	void recRecieveEvent(GameObject* obj, const Event& e);

	bool saveSceneNow(const char* name, const char* path = NULL);
	bool loadSceneNow(const char* name, const char* path = NULL);

	void onPlay();
	void onPause();
	void onStop();

public:
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;
	bool showTree = false;

	JOctree* sceneTree = NULL;

private:

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;

	bool mustSaveScene = false;
	bool mustLoadScene = false;
	std::string currentScene;
	//TODO: List/Map of all scenes?? Should a scene be a resource and the list/map be managed as a resource?
	/**
		For now as a scene is basically a root game object, will leave it here but must check this in the future.
	*/
};

#endif // !__MODULEGOMANAGER_H__