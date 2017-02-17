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
	type = CMP_MATERIAL;
	name.assign("Material");
}


Material::~Material()
{
	ClearMaterial();
}

void Material::Init()
{

}

void Material::Update(float dt)
{

}

void Material::CleanUp()
{

}

bool Material::SaveCMP(FileParser& sect)
{
	bool ret = true;

	sect.AddInt("comp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("UUID", id);
	sect.AddInt("go_UUID", object->GetGOId());
	sect.AddColor("mat_col", color);

	if (textureResource)
	{
		sect.AddBool("have_res", true);
		sect.AddInt("resource_id", textureResource->GetUID());
		sect.AddString("resource_exported_file", textureResource->exportedFile.c_str());
		sect.AddString("resource_original_file", textureResource->originalFile.c_str());
	}
	else
		sect.AddBool("have_res", false);

	return ret;
}

bool Material::LoadCMP(FileParser& sect)
{
	bool ret = true;

	selfActive = sect.GetBool("active", true);
	id = sect.GetInt("UUID", 0);

	color = sect.GetColor("mat_col", Yellow);

	if (sect.GetBool("have_res", false))
	{
		SetResource(sect.GetInt("resource_id", 0));
	}

	return ret;
}


//-------------------------------------

void Material::SetResource(UID resUID)
{
	ResourceTexture* res = (ResourceTexture*)app->resourceManager->GetResourceFromUID(resUID);

	if (res)
	{
		if (!res->IsInMemory())
		{
			//If is not in memory load it.
			if (!app->resourceManager->LoadResource(res))
			{
				_LOG(LOG_ERROR, "Error loading resource '%s'.", res->exportedFile.c_str());
			}
			else
			{
				_LOG(LOG_STD, "Just loaded resource '%s'.", res->exportedFile.c_str());
			}
		}
		else
			res->AddInstance();

		textureResource = res;
	}
}

void Material::ClearMaterial()
{
	if (textureResource)
	{
		app->resourceManager->OnResourceRemove(textureResource);
		textureResource = nullptr;
	}
}