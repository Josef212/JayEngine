#include "Application.h"
#include "ResourceTexture.h"
#include "ModuleFileSystem.h"

#include "OpenGL.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")

ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_TEXTURE;
}


ResourceTexture::~ResourceTexture()
{
}

/**
	TODO: Should each resource load the texture?? Or importer should have a method that load 
	textures with a tex resource as parameter.
	
	If the resource load the texture should it really have the file name parameter?? Doesnt it have 
	the exported file name??
	For now will keep like this but must clean a lot all this code.
*/
void ResourceTexture::loadTexture(const char* fileName, const char* path) 
{
	if (!fileName)
	{
		_LOG(LOG_ERROR, "Error loading a texture .dds: Invalid file name or texture resource.");
		return;
	}

	//Assume fileName is clear
	std::string realPath; //TODO: Dont use std

	if(!path)
		realPath.assign(DEFAULT_TEXTURE_SAVE_PATH);
	else
		realPath.assign(path);

	realPath.append(fileName);

	_LOG(LOG_INFO, "Loading texture: %s.", realPath.c_str());

	//TODO: Actually load the texture

	char* buffer = NULL; //TODO: Would be a good practise to set to NULL all buffer char pointers
	uint size = app->fs->load(realPath.c_str(), &buffer);

	if (buffer && size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoadL(IL_DDS, (const void*)buffer, size))
		{
			textureGlID = ilutGLBindTexImage();
			ilDeleteImages(1, &image);
		}
	}
	else
		_LOG(LOG_ERROR, "Error loading dds: %s.", fileName);

	RELEASE_ARRAY(buffer);

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