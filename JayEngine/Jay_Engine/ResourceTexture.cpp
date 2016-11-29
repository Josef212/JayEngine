#include "ResourceTexture.h"

#include "OpenGL.h"


ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_TEXTURE;
}


ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::loadTexture(const char* fileName, const char* path)
{
	if (!fileName)
	{
		_LOG(LOG_ERROR, "Error loading a texture .dds: Invalid file name or texture resource.");
		return;
	}

	//Assume fileName is clear
	std::string realPath;

	if(!path)
		realPath.assign(DEFAULT_TEXTURE_SAVE_PATH);
	else
		realPath.assign(path);

	realPath.append(fileName);

	_LOG(LOG_INFO, "Loading texture: %s.", realPath.c_str());

	//TODO: Actually load the texture
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