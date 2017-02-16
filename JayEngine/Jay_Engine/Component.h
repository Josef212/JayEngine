#ifndef __COMPONENT__
#define __COMPONENT__

#include "Globals.h"
#include "Math.h"
#include <string>

class GameObject;
class FileParser;
class Transform;

enum ComponentType
{
	CMP_UNKNOWN = -1,
	CMP_TRANSFORMATION,
	CMP_MESH,
	CMP_MATERIAL,
	CMP_CAMERA,
	CMP_SPRITE_RENDERER,
	CMP_MAP
};

class Component
{
public:
	ComponentType type = CMP_UNKNOWN;
	bool removeFlag = false;

protected:
	std::string name;
	bool selfActive = true;
	GameObject* object = nullptr;
	int id = -1;

public:
	Component(GameObject* gObj, int id) : object(gObj), id(id)
	{}
	
	virtual ~Component()
	{}

	virtual void Enable() { selfActive = true; }
	virtual void Disable() { selfActive = false; }
	void SwitchActive() { selfActive = !selfActive; }

	virtual void Init() {}
	virtual void Update(float dt) {}
	virtual void CleanUp() {}

	const bool IsEnable()const { return selfActive; }
	const char* GetName() { return name.c_str(); }
	void SetName(const char* str) { if (str) name.assign(str); }
	int GetId() { return id; }

	void Remove() { removeFlag = true; }

	virtual void OnGameObjectDestroyed() {}

	virtual void GetBox(AABB& box)const {}
	virtual void OnTransformUpdate(Transform* trans) {}

	virtual void DebugDraw() {}

	virtual bool SaveCMP(FileParser& sect) { return false; }
	virtual bool LoadCMP(FileParser& sect) { return false; }

	virtual void SetResource(UID resUID) {}


};

#endif // !__COMPONENT__