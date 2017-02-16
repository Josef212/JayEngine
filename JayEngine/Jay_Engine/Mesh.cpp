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

	sect.addInt("comp_type", (int)type);
	sect.addBool("active", selfActive);
	sect.addInt("UUID", id);
	sect.addInt("go_UUID", object->GetGOId());

	if (meshResource)
	{
		sect.addBool("have_res", true);
		sect.addInt("resource_id", meshResource->GetUID());
		sect.addString("resource_exported_file", meshResource->exportedFile.c_str());
		sect.addString("resource_original_file", meshResource->originalFile.c_str());
	}
	else
		sect.addBool("have_res", false);

	sect.addBool("wireframe", renderWireframe);
	sect.addBool("normals", renderNormals);

	return ret;
}

bool Mesh::LoadCMP(FileParser& sect)
{
	bool ret = true;

	selfActive = sect.getBool("active", true);
	id = sect.getInt("UUID", 0);

	if (sect.getBool("have_res", false))
	{
		SetResource(sect.getInt("resource_id", 0));
	}

	renderWireframe = sect.getBool("wireframe", false);
	renderNormals = sect.getBool("normals", false);

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