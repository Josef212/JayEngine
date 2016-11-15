#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <string>

enum ResourceType
{
	RESOURCE_UNKNOWN = -1,
	RESOURCE_MESH = 0,
	RESOURCE_MATERIAL
};

class Resource
{
public:
	Resource(UID uuid);
	virtual ~Resource();

	ResourceType getResourceType()const;
	UID getUID()const;
	const char* getOriginalFile();
	const char* getExportedFile();
	bool isInMemory()const;
	bool loadToMemory();
	bool removeFromMemory();
	uint countReferences()const;

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