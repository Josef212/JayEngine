#include "Globals.h"
#include "Application.h"
#include "ResourceMesh.h"

#include "ImporterMesh.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"

#include "OpenGL.h"

ResourceMesh::ResourceMesh(UID uuid) : Resource(uuid, RESOURCE_MESH)
{
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	return app->resourceManager->meshImporter->LoadResource(this);
}

void ResourceMesh::Save(FileParser& file)
{
	Resource::Save(file);
}

void ResourceMesh::Load(FileParser& file)
{
	Resource::Load(file);
}

bool ResourceMesh::RemoveFromMemory()
{
	if (idIndices > 0) glDeleteBuffers(1, &idIndices);
	if (idVertices > 0) glDeleteBuffers(1, &idVertices);
	if (idNormals > 0) glDeleteBuffers(1, &idNormals);
	if (idTexCoords > 0) glDeleteBuffers(1, &idTexCoords);
	if (idColors > 0) glDeleteBuffers(1, &idColors);

	numIndices = 0;
	numVertices = 0;

	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(texCoords);
	RELEASE_ARRAY(colors);

	return true;
}