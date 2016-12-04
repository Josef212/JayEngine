#include "Application.h"
#include "ResourceTexture.h"


ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_TEXTURE;
}


ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::loadToMemory()
{
	bool ret = false;

	return ret;
}

bool ResourceTexture::removeFromMemory()
{
	return false;
}