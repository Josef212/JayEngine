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
	bool removeFlag = false;

protected:
	ComponentType type = CMP_UNKNOWN;
	bool selfActive = true;
	GameObject* object = nullptr;

public:
	Component(GameObject* gObj);
	virtual ~Component();

	//-----------------------------------

	bool IsActive()const;
	void Enable();
	void Disable();
	void SwitchActive();
	void SetSelfActive(bool set);

	void Destroy();

	//-----------------------------------
	
	ComponentType GetType()const;
	const char* GetTypeStr()const;
	GameObject* GetGameObject()const;

	//-----------------------------------

	virtual void GetBox(AABB& box)const {}

	//-----------------------------------

	virtual void OnEnable() {}
	virtual void OnDisable() {}
	
	virtual void OnStart() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnFinish() {}

	virtual void OnDebugDraw() {}

	virtual void OnPlay() {}
	virtual void OnStop() {}
	virtual void OnPause() {}
	virtual void OnUnPause() {}

	virtual void OnGameObjectDestroyed() {}

	virtual void OnTransformUpdate(Transform* trans) {}

	//-----------------------------------

	virtual bool SaveCMP(FileParser& sect)const = 0;
	virtual bool LoadCMP(FileParser* sect) = 0;

};

#endif // !__COMPONENT__