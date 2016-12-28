#ifndef __RESOURCE_H__
#define __RESOURCE_H__

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
	Resource(UID uuid, ResourceType resType = RESOURCE_UNKNOWN);
	virtual ~Resource();

	ResourceType getResourceType()const;

	UID getUID()const;
	void setUID(UID uid); //NOTE: Care when use it. //TODO: Check for other ways

	const char* getOriginalFile()const;
	const char* getExportedFile()const;

	bool isInMemory()const;

	virtual bool loadToMemory();
	virtual bool removeFromMemory();

	uint countReferences()const;

	void addInstance();
	void removeInstance();

private:

public:
	std::string originalFile;
	std::string exportedFile;

protected:
	ResourceType resType = RESOURCE_UNKNOWN;
	UID uuid = 0;

	uint instancesInMemory = 0;
};

#endif // !__RESOURCE_H__