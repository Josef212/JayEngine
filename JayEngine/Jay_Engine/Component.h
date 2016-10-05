#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

class GameObject;

enum ComponentType
{
	UNKNOWN = 0,
	TRANSFORMATION,
	MESH,
	MATERIAL
};

class Component
{
public:
	Component(GameObject* gObj);
	virtual ~Component();

	const bool isEnable()const;

	virtual void enable();
	virtual void disable();

	virtual void init();
	virtual void update(float dt);
	virtual void cleanUp();

public:
	ComponentType type = UNKNOWN;

protected:
	bool active = true;
	uint componentLimit = 0, componentInstances = 0;//TODO: thinking to put this data on another structure to control it
	GameObject* object = NULL;
};

#endif // !__COMPONENT_H__