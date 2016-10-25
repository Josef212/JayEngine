#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include <string>

class GameObject;

enum ComponentType
{
	UNKNOWN = 0,
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

public:
	ComponentType type = UNKNOWN;

protected:
	std::string name;
	bool active = true;
	uint componentLimit = 0, componentInstances = 0;//TODO: thinking to put this data on another structure to control it
	GameObject* object = NULL;
	int id = -1;
};

#endif // !__COMPONENT_H__