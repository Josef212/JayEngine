#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include "Math.h"
#include <string>

class GameObject;
class FileParser;
class Transform;

enum ComponentType
{
	UNKNOWN = -1,
	TRANSFORMATION,
	MESH,
	MATERIAL,
	CAMERA
};

class Component
{
public:
	Component(GameObject* gObj, int id);
	virtual ~Component();
	
	virtual void enable();
	virtual void disable();
	void switchActive();

	virtual void init();
	virtual void update(float dt);
	virtual void cleanUp();

	const bool isEnable()const;
	const char* getName();
	void setName(const char* str);
	int getId();

	void remove();

	virtual void onGameObjectDestroyed() {}

	virtual void getBox(AABB& box)const {}
	virtual void onTransformUpdate(Transform* trans) {}

	virtual void debugDraw();

	virtual bool saveCMP(FileParser& sect);
	virtual bool loadCMP(FileParser& sect);

	virtual void setResource(UID resUID) {}

public:
	ComponentType type = UNKNOWN;
	bool removeFlag = false;

protected:
	std::string name;
	bool active = true;
	GameObject* object = NULL;
	int id = -1;
};

#endif // !__COMPONENT_H__