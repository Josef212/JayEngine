#include "Material.h"

#include "Application.h"
#include "FileParser.h"
#include "GameObject.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"
#include "ResourceShader.h"
#include "ModuleResourceManager.h"
#include "ImporterTexture.h"


Material::Material(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MATERIAL;
	name.assign("Material");
}


Material::~Material()
{
	clearMaterial();
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

bool Material::saveCMP(FileParser& sect)
{
	bool ret = true;

	sect.addInt("comp_type", (int)type);
	sect.addBool("active", active);
	sect.addInt("UUID", id);
	sect.addInt("go_UUID", object->getGOId());
	sect.addColor("mat_col", color);

	if (textureResource)
	{
		sect.addBool("have_res", true);
		sect.addInt("resource_id", textureResource->getUID());
		sect.addString("resource_exported_file", textureResource->getExportedFile());
		sect.addString("resource_original_file", textureResource->getOriginalFile());
	}
	else
		sect.addBool("have_res", false);

	return ret;
}

bool Material::loadCMP(FileParser& sect)
{
	bool ret = true;

	active = sect.getBool("active", true);
	id = sect.getInt("UUID", 0);

	color = sect.getColor("mat_col", Yellow);

	if (sect.getBool("have_res", false))
	{
		setResource(sect.getInt("resource_id", 0));
	}

	return ret;
}


//-------------------------------------

void Material::setResource(UID resUID)
{
	ResourceTexture* res = (ResourceTexture*)app->resourceManager->getResourceFromUID(resUID);

	if (res)
	{
		if (!res->isInMemory())
		{
			//If is not in memory load it.
			if (!app->resourceManager->loadResource(res))
			{
				_LOG(LOG_ERROR, "Error loading resource '%s'.", res->getExportedFile());
			}
			else
			{
				_LOG(LOG_STD, "Just loaded resource '%s'.", res->getExportedFile());
			}
		}
		else
			res->addInstance();

		textureResource = res;
	}
}

void Material::clearMaterial()
{
	if (textureResource)
	{
		app->resourceManager->onResourceRemove(textureResource);
		textureResource = NULL;
	}
}