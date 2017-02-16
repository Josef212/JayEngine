#ifndef __RESOURCE__
#define __RESOURCE__

#include "Globals.h"
#include <string>

enum ResourceType
{
	RESOURCE_UNKNOWN = -1,
	RESOURCE_MESH = 0,
	RESOURCE_TEXTURE,
	RESOURCE_SHADER,
	RESOURCE_SCENE,
	RESOURCE_MATERIAL,
	RESOURCE_AUDIO
};

class Resource
{
public:
	Resource(UID uuid, ResourceType resType = RESOURCE_UNKNOWN) : uuid(uuid), resType(resType)
	{}

	virtual ~Resource()
	{}

	ResourceType GetType()const { return resType; }
	UID GetUID()const { return uuid; }

	bool IsInMemory()const { return instancesInMemory > 0; }

	virtual bool LoadToMemory() { return false; }
	virtual bool RemoveFromMemory() { return false; }

	uint CountReferences()const { return instancesInMemory; }

	void AddInstance() { ++instancesInMemory; }
	void RemoveInstance() { --instancesInMemory; }

private:

public:
	std::string originalFile;
	std::string exportedFile;

protected:
	ResourceType resType = RESOURCE_UNKNOWN;
	UID uuid = 0;

	uint instancesInMemory = 0;
};

#endif // !__RESOURCE__