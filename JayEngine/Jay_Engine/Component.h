#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include <string>

class GameObject;
class FileParser;

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

	virtual void debugDraw();

	virtual bool saveCMP(FileParser* sect);
	virtual bool loadCMP(FileParser* sect);

public:
	ComponentType type = UNKNOWN;

protected:
	std::string name;
	bool active = true;
	GameObject* object = NULL;
	int id = -1;
};

#endif // !__COMPONENT_H__