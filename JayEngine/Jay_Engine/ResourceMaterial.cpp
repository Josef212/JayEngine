#include "ResourceMaterial.h"



ResourceMaterial::ResourceMaterial(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_MATERIAL;
}


ResourceMaterial::~ResourceMaterial()
{
}

bool ResourceMaterial::loadToMemory()
{
	bool ret = false;

	return ret;
}

bool ResourceMaterial::removeFromMemory()
{
	return false;
}