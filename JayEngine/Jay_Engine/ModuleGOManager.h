#ifndef __MODULEGOMANAGER_H__
#define __MODULEGOMANAGER_H__

#include "Module.h"
#include <map>

class GameObject;
class Component;
struct aiScene;
struct aiNode;
struct aiMesh;

class JQuadTree;
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


	bool deleteGameObject(GameObject* toDel);

	GameObject* loadCube(); //DEL_COM

	void drawDebug();

	void makeGOShowAABoxRec(GameObject* obj, bool show);
	void makeGOShowOBoxRec(GameObject* obj, bool show);

	void insertGameObjectToTree(GameObject* obj);
	void eraseGameObjectFromTree(GameObject* obj); //DEL_COM

private:

public:
	int nextGOId = 0;
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;
	bool showTree = false;

	//JQuadTree* sceneTree = NULL; //DEL_COM
	JOctree* sceneTree = NULL; //DEL_COM

	GameObject* mainCamera = NULL;

	//-------
	std::map<std::string, uint> texturesLoaded;  //textures paths and idBuffers

private:

	GameObject* sceneRootObject = NULL;
	GameObject* selected = NULL;

	uint indexGO = 0;
};

#endif // !__MODULEGOMANAGER_H__