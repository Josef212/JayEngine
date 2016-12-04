#include "Resource.h"



Resource::Resource(UID uuid) : uuid(uuid)
{
}


Resource::~Resource()
{
}

ResourceType Resource::getResourceType()const
{
	return resType;
}

UID Resource::getUID()const
{
	return uuid;
}

void Resource::setUID(UID uid)
{
	if (uid > 0)
		uuid = uid;
}

const char* Resource::getOriginalFile()
{
	return originalFile.c_str();
}

const char* Resource::getExportedFile()
{
	return exportedFile.c_str();
}

bool Resource::isInMemory()const
{
	return (instancesInMemory > 0);
}

bool Resource::loadToMemory()
{
	return false;
}

bool Resource::removeFromMemory()
{
	return false;
}

uint Resource::countReferences()const
{
	return instancesInMemory;
}

void Resource::addInstance()
{
	++instancesInMemory;
}

void Resource::removeInstance()
{
	--instancesInMemory;
}