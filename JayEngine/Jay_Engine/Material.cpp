#include "Material.h"

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

	uint id = ilutGLLoadImage(realPath);

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