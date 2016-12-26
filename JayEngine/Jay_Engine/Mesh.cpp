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
	//if(meshResource)
		//app->resourceManager->removeResource(meshResource->getUID());
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
	if(meshResource)
		clearMesh();
}

void Mesh::clearMesh()
{
}

void Mesh::getBox(AABB& box)const
{
	if (meshResource)
		box.Enclose(meshResource->aabb);
}

ResourceMesh* Mesh::createAnEmptyMeshRes()
{
	if (!meshResource)
	{
		meshResource = (ResourceMesh*)app->resourceManager->createNewResource(RESOURCE_MESH);
	}
	
	return meshResource;
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
		uint id = sect.getInt("resource_id", 0);
		ResourceMesh* tmp = (ResourceMesh*)app->resourceManager->getResourceFromUID(id);
		if (tmp)
		{
			meshResource = tmp;
			tmp->addInstance();
		}
		else
		{
			createAnEmptyMeshRes();

			if (id > 0)
				meshResource->setUID(id);

			meshResource->loadMeshResource(sect.getString("resource_exported_file", NULL));
			meshResource->originalFile.assign(sect.getString("resource_original_file", NULL));
			meshResource->loadToMemory();
		}

	}

	renderWireframe = sect.getBool("wireframe", false);
	renderNormals = sect.getBool("normals", false);

	return ret;
}