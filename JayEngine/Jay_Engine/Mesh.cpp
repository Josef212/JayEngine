#include "Application.h"
#include "Mesh.h"

#include "FileParser.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"


Mesh::Mesh(GameObject* gObj) : Component(gObj)
{
	type = CMP_MESH;
}


Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::OnStart()
{
	
}

void Mesh::ClearMesh()
{
}

void Mesh::GetBox(AABB& box)const
{
	ResourceMesh* r = (ResourceMesh*)resource;
	if (resource)
		box.Enclose(r->aabb);
}

bool Mesh::SaveCMP(FileParser& sect)const
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

	sect.AddColor("tint", tint);

	sect.AddBool("wireframe", renderWireframe);
	sect.AddBool("normals", renderNormals);

	return ret;
}

bool Mesh::LoadCMP(FileParser* sect)
{
	bool ret = true;

	selfActive = sect->GetBool("active", true);

	if (sect->GetBool("have_res", false))
		ComponentResource::OnLoadRes(sect);

	tint = sect->GetColor("tint", White);

	renderWireframe = sect->GetBool("wireframe", false);
	renderNormals = sect->GetBool("normals", false);

	return ret;
}


//--------------------------

bool Mesh::SetResource(UID resUID)
{
	bool ret = false;

	if (resUID != 0)
	{
		Resource* res = app->resourceManager->GetResourceFromUID(resUID);
		if (res && res->GetType() == RESOURCE_MESH)
		{
			if (res->LoadToMemory())
			{
				resource = res;
				object->RecalcBox();
				ret = true;
			}
		}
	}

	return ret;
}