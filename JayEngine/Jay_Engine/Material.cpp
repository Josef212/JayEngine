#include "Material.h"

#include "Application.h"
#include "FileParser.h"
#include "GameObject.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"
#include "ModuleResourceManager.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"


Material::Material(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MATERIAL;
	name.assign("Material");
}


Material::~Material()
{
	textures.clear();
	paths.clear();
}

void Material::enable()
{
	if (!active)
		active = true;
}

void Material::disable()
{
	if (active)
		active = false;
}

void Material::init()
{

}

void Material::update(float dt)
{

}

void Material::cleanUp()
{

}

int Material::loadTexture(char* file, char* path)
{
	int ret = -1;

	if (!file)
	{
		_LOG(LOG_ERROR, "Error while loading a texture: File is NULL");
		return ret;
	}

	char* realPath = new char[256];

	if (path)
		strcpy_s(realPath, 256, path);
	else
		strcpy_s(realPath, 256, DEFAULT_TEXTURES_PATH);

	strcat_s(realPath, 256, "/");
	strcat_s(realPath, 256, file);

	_LOG(LOG_STD, "Loading a texture from: %s", realPath);

	
	/*ILubyte* buffer;
	uint fileSize = app->fs->load(realPath, (char**)&buffer);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, fileSize))//TODO: change type because DDS images should be load
	{
		ILuint devilError = ilGetError();
		if (devilError != IL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Error while loading texture '%s'. Devil:%s\n", realPath, iluErrorString(devilError));
		}
	}*/

	uint id = 0;

	for (std::map<std::string, uint>::iterator it = app->goManager->texturesLoaded.begin(); it != app->goManager->texturesLoaded.end(); ++it)
	{
		if ((*it).first == realPath)
		{
			id = (*it).second;
			break;
		}
	}

	if (id <= 0)
	{
		uint id = ilutGLLoadImage(realPath);
		app->goManager->texturesLoaded.insert(std::pair<std::string, uint>(std::string(realPath), id));
		if (id > 0)
		{
			textures.push_back(id);
			ret = textures.size() - 1;
			paths.insert(std::pair<std::string, int>(std::string(realPath), ret));
		}
	}

	if (id > 0)
	{
		textures.push_back(id);
		ret = textures.size() - 1;
		paths.insert(std::pair<std::string, int>(std::string(realPath), ret));
	}
	else
	{
		_LOG(LOG_ERROR, "Error loading texture %s", realPath);
		ILuint devilError = ilGetError();
		if (devilError != IL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Error while loading a texture, devil: %s\n", iluErrorString(devilError));
		}
	}

	RELEASE_ARRAY(realPath);

	return ret;
}

int Material::getTexture(int index)
{
	int ret = -1;

	if (isEnable() && IS_INSIDE(index, textures.size() - 1, 0))
		ret = textures[index];

	return ret;
}

ResourceTexture* Material::createAnEmptyMaterialRes()
{
	if(!textureResource)
		textureResource = (ResourceTexture*)app->resourceManager->createNewResource(RESOURCE_TEXTURE);

	return textureResource;
}

bool Material::saveCMP(FileParser* sect)
{
	bool ret = true;

	sect->addInt("comp_type", (int)type);
	sect->addBool("active", active);
	sect->addInt("UUID", id);
	sect->addInt("go_UUID", object->getGOId());
	sect->addColor("mat_col", color);

	if (textureResource)
	{
		sect->addInt("resource_id", textureResource->getUID());
		sect->addString("resource_exported_file", textureResource->getExportedFile());
		sect->addString("resource_original_file", textureResource->getOriginalFile());
	}

	return ret;
}

bool Material::loadCMP(FileParser* sect)
{
	bool ret = true;

	return ret;
}