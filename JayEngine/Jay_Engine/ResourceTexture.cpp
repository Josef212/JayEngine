#include "Application.h"
#include "ResourceTexture.h"

#include "ModuleResourceManager.h"

#include "OpenGL.h"

ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid)
{
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
	if (textureGlID > 0) 
	{
		glDeleteBuffers(1, &textureGlID); 
		return true;
	}
	return false;
}