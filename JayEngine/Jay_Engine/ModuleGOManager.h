#ifndef __MODULEGOMANAGER__
#define __MODULEGOMANAGER__

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

	bool Init(FileParser* conf)override;
	bool Start()override;
	update_status PreUpdate(float dt)override;
	update_status Update(float dt)override;
	update_status PostUpdate(float dt)override;
	bool CleanUp()override;

	void Draw();

	void RemoveFlaggedGO();

	GameObject* GetSceneroot()const;
	GameObject* GetGameObjectFromId(UID id);


	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* CreateEmptyGO();
	GameObject* CreateCamera();

	GameObject* GetSelected()const;
	void Select(GameObject* toSelect);

	GameObject* ValidateGO(const GameObject* point)const;

	GameObject* LoadPrefab(const char* file, const char* path = nullptr); //Must be a json wich contains all .jof and .dds, etc

	void CleanRoot();
	void CleanRootNow();

	GameObject* LoadCube();

	void DrawDebug();

	void MakeGOShowAABoxRec(GameObject* obj, bool show);
	void MakeGOShowOBoxRec(GameObject* obj, bool show);

	void InsertGameObjectToTree(GameObject* obj);
	void EraseGameObjectFromTree(GameObject* obj); //DEL_COM

	void SaveScene();
	void LoadScene();

	void LoadSceneOrPrefabs(const FileParser& file);

	bool SetCurrentScene(const char* scene);
	const char* GetCurrentScene();

	void OnGlobalEvent(const Event& e);

private:
	GameObject* RecFindGO(UID id, GameObject* go);
	void RecRecieveEvent(GameObject* obj, const Event& e);

	bool SaveSceneNow(const char* name, const char* path = nullptr);
	bool LoadSceneNow(const char* name, const char* path = nullptr);

	void OnPlay();
	void OnPause();
	void OnStop();

public:
	bool showEnclosingBoxes = false;
	bool showOrientedBoxes = false;
	bool showTree = false;

	JOctree* sceneTree = nullptr;

private:

	GameObject* sceneRootObject = nullptr;
	GameObject* selected = nullptr;

	bool mustSaveScene = false;
	bool mustLoadScene = false;
	std::string currentScene;
	//TODO: List/Map of all scenes?? Should a scene be a resource and the list/map be managed as a resource?
	/**
		For now as a scene is basically a root game object, will leave it here but must check this in the future.
	*/
};

#endif // !__MODULEGOMANAGER__