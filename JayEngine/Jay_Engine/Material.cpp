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


Material::Material(GameObject* gObj) : Component(gObj)
{
	type = CMP_MATERIAL;
}


Material::~Material()
{
	ClearMaterial();
}

bool Material::SaveCMP(FileParser& sect)const
{
	bool ret = true;

	if (resource)
	{
		sect.AddBool("have_res", true);
		ComponentResource::OnSaveRes(sect);
	}
	else
	{
		sect.AddBool("have_res", false);
	}

	return ret;
}

bool Material::LoadCMP(FileParser* sect)
{
	bool ret = true;

	selfActive = sect->GetBool("active", true);

	if (sect->GetBool("have_res", false))
		ComponentResource::OnLoadRes(sect);

	return ret;
}


//-------------------------------------

bool Material::SetResource(UID resUID)
{
	bool ret = false;

	if (resUID != 0)
	{
		Resource* res = app->resourceManager->GetResourceFromUID(resUID);
		if (res && res->GetType() == RESOURCE_TEXTURE)
		{
			if (res->LoadToMemory())
			{
				resource = (ResourceTexture*)res;
				ret = true;
			}
		}
	}

	return ret;
}






void Material::ClearMaterial()
{
	if (resource)
	{
		resource = nullptr;
	}
}