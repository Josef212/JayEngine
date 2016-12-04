#include "Material.h"

#include "Application.h"
#include "FileParser.h"
#include "GameObject.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"

#include "ResourceTexture.h"
#include "ModuleResourceManager.h"
#include "ImporterTexture.h"


Material::Material(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MATERIAL;
	name.assign("Material");
}


Material::~Material()
{
	app->resourceManager->removeResource(textureResource->getUID());
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



int Material::getTexture(int index)
{
	int ret = -1;

	return ret;
}

ResourceTexture* Material::createAnEmptyMaterialRes()
{
	if(!textureResource)
		textureResource = (ResourceTexture*)app->resourceManager->createNewResource(RESOURCE_TEXTURE);

	return textureResource;
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
		uint id = sect.getInt("resource_id", 0);
		ResourceTexture* tmp = (ResourceTexture*)app->resourceManager->getResourceFromUID(id);
		if (tmp)
			textureResource = tmp;
		else
		{

			createAnEmptyMaterialRes();

			if (id > 0)
				textureResource->setUID(id);

			textureResource->exportedFile.assign(sect.getString("resource_exported_file", NULL));
			textureResource->originalFile.assign(sect.getString("resource_original_file", NULL));
			app->resourceManager->textureImporter->loadTexture(textureResource);

		}
	}

	return ret;
}