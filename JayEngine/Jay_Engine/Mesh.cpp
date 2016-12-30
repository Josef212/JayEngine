#include "Application.h"
#include "Mesh.h"

#include "FileParser.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"


Mesh::Mesh(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MESH;
	name.assign("Mesh");
}


Mesh::~Mesh()
{
	clearMesh();
}

void Mesh::enable()
{
	if (!active)
		active = true;
}

void Mesh::disable()
{
	if (active)
		active = false;
}

void Mesh::init()
{
	
}

void Mesh::update(float dt)
{

}

void Mesh::cleanUp()
{
	
}

void Mesh::clearMesh()
{
	if (meshResource)
	{
		app->resourceManager->onResourceRemove(meshResource);
		meshResource = NULL;
	}
}

void Mesh::getBox(AABB& box)const
{
	if (meshResource)
		box.Enclose(meshResource->aabb);
}

bool Mesh::saveCMP(FileParser& sect)
{
	bool ret = true;

	sect.addInt("comp_type", (int)type);
	sect.addBool("active", active);
	sect.addInt("UUID", id);
	sect.addInt("go_UUID", object->getGOId());

	if (meshResource)
	{
		sect.addBool("have_res", true);
		sect.addInt("resource_id", meshResource->getUID());
		sect.addString("resource_exported_file", meshResource->exportedFile.c_str());
		sect.addString("resource_original_file", meshResource->originalFile.c_str());
	}
	else
		sect.addBool("have_res", false);

	sect.addBool("wireframe", renderWireframe);
	sect.addBool("normals", renderNormals);

	return ret;
}

bool Mesh::loadCMP(FileParser& sect)
{
	bool ret = true;

	active = sect.getBool("active", true);
	id = sect.getInt("UUID", 0);

	if (sect.getBool("have_res", false))
	{
		setResource(sect.getInt("resource_id", 0));
	}

	renderWireframe = sect.getBool("wireframe", false);
	renderNormals = sect.getBool("normals", false);

	return ret;
}


//--------------------------

void Mesh::setResource(UID resUID)
{
	//TODO: if there is already a resource, notify to manager and check if there are no more instances to remove from memory

	ResourceMesh* res = (ResourceMesh*)app->resourceManager->getResourceFromUID(resUID);

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

		meshResource = res;
	}

}