#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

enum ResourceTypes
{
	RESOURCE_UNKNOWN = -1,
	RESOURCE_MESH = 0,
	RESOURCE_MATERIAL
};

class Resource
{
public:
	Resource(uint UID);
	virtual ~Resource();

private:

public:

protected:
	ResourceTypes resType = RESOURCE_UNKNOWN;
	uint UID = 0;
};

#endif // !__RESOURCE_H__