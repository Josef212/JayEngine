#include "Application.h"
#include "ResourceTexture.h"

#include "ImporterTexture.h"
#include "ModuleResourceManager.h"

#include "OpenGL.h"

ResourceTexture::ResourceTexture(UID uuid) : Resource(uuid, RESOURCE_TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::Save(FileParser& file)
{
	Resource::Save(file);
}

void ResourceTexture::Load(FileParser& file)
{
	Resource::Load(file);
}

bool ResourceTexture::LoadInMemory()
{
	return app->resourceManager->textureImporter->LoadResource(this);
}

bool ResourceTexture::RemoveFromMemory()
{
	if (textureGlID > 0) 
	{
		glDeleteBuffers(1, &textureGlID); 
		return true;
	}
	return false;
}

const char* ResourceTexture::GetFormatStr()const
{
	static const char* formats[] = {
		"color index", "rgb", "rgba", "bgr", "bgra", "luminance", "unknown" };

	if (format >= 0 && format <= UNKNOWN)
		return formats[format];
	else
		return nullptr;
}