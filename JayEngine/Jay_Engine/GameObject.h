#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include "Globals.h"
#include "Component.h"
#include "Math.h"
#include <vector>
#include <string>
#include <map>

class Transform;
class Mesh;
class Material;
class Camera;

class FileParser;

class GameObject
{
public:
	GameObject(GameObject* parent, UID id);
	virtual ~GameObject();

	void Update(float dt);

	//------
	void OnStart();
	void OnFinish();
	void OnEnable();
	void OnDisable();

	void OnPlay();
	void OnStop();
	void OnPause();
	void OnUnPause();

	void OnGameObjectDestroyed();
	//------

	UID GetGOId()const;
	const char* GetName()const;
	void SetName(const char* str);

	//------

	GameObject* AddChild();
	Component* AddComponent(ComponentType type);

	Component* GetComponent(ComponentType type)const;
	std::vector<Component*> GetComponents(ComponentType type);
	bool HasComponent(ComponentType type)const;
	uint CountComponents(ComponentType type)const;

	GameObject* GetParent() const;
	void SetNewParent(GameObject* newParent, bool force = false);

	void Draw(bool drawChilds);
	void DrawDebug();

	void Remove();

	bool IsGOActive();
	void SetSelfActive(bool set);
	void Enable();
	void Disable();

	//------

	void RecCalcTransform(const float4x4& parentTrans, bool force = false);
	void RecCalcBoxes();
	void RecalcBox();

	bool RecRemoveFlagged();

	//------
	
	bool SaveGO(FileParser& file, std::map<uint, uint>* duplicate = nullptr)const;
	bool LoadGO(FileParser* file, std::map<GameObject*, uint>& relations);

private:


public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	AABB enclosingBox;
	OBB orientedBox;

	bool drawEnclosingAABB = false;
	bool drawOrientedBox = false;

	bool removeFlag = false;

	/**Just a pointer to transformation component
	in order to agile transform search. May be transform info should be in game object class */
	Transform* transform = nullptr;

private:
	std::string name;
	UID id = 0;
	GameObject* parent = nullptr;
	bool selfActive = true;

	bool wasDirty = true;

};

#endif // !__GAMEOBJECT__