#include "Application.h"
#include "Mesh.h"

#include "FileParser.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"


Mesh::Mesh(GameObject* gObj, int id) : Component(gObj, id)
{
	type = CMP_MESH;
	name.assign("Mesh");
}


Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::Init()
{
	
}

void Mesh::Update(float dt)
{

}

void Mesh::CleanUp()
{
	
}

void Mesh::ClearMesh()
{
	if (meshResource)
	{
		app->resourceManager->OnResourceRemove(meshResource);
		meshResource = nullptr;
	}
}

void Mesh::GetBox(AABB& box)const
{
	if (meshResource)
		box.Enclose(meshResource->aabb);
}

bool Mesh::SaveCMP(FileParser& sect)
{
	bool ret = true;

	sect.AddInt("comp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("UUID", id);
	sect.AddInt("go_UUID", object->GetGOId());

	if (meshResource)
	{
		sect.AddBool("have_res", true);
		sect.AddInt("resource_id", meshResource->GetUID());
		sect.AddString("resource_exported_file", meshResource->exportedFile.c_str());
		sect.AddString("resource_original_file", meshResource->originalFile.c_str());
	}
	else
		sect.AddBool("have_res", false);

	sect.AddBool("wireframe", renderWireframe);
	sect.AddBool("normals", renderNormals);

	return ret;
}

bool Mesh::LoadCMP(FileParser& sect)
{
	bool ret = true;

	selfActive = sect.GetBool("active", true);
	id = sect.GetInt("UUID", 0);

	if (sect.GetBool("have_res", false))
	{
		SetResource(sect.GetInt("resource_id", 0));
	}

	renderWireframe = sect.GetBool("wireframe", false);
	renderNormals = sect.GetBool("normals", false);

	return ret;
}


//--------------------------

void Mesh::SetResource(UID resUID)
{
	//TODO: if there is already a resource, notify to manager and check if there are no more instances to remove from memory

	ResourceMesh* res = (ResourceMesh*)app->resourceManager->GetResourceFromUID(resUID);

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

		meshResource = res;
	}

}