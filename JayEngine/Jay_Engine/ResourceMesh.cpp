#include "Globals.h"
#include "Application.h"
#include "ResourceMesh.h"

#include "ModuleFileSystem.h"

#include "OpenGL.h"

ResourceMesh::ResourceMesh(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_MESH;
}


ResourceMesh::~ResourceMesh()
{
	clearResMesh();
}

void ResourceMesh::loadMeshResource(const char* fileName)
{
	if (!fileName)
	{
		_LOG(LOG_ERROR, "Error loading a mesh .jof: Invalid file name or mesh resource.");
		return;
	}

	std::string realName(DEFAULT_MESH_SAVE_PATH);
	realName.append(fileName);

	_LOG(LOG_INFO, "Loading mesh: %s.", realName.c_str());

	char* data;
	uint size = app->fs->load(realName.c_str(), &data);

	if (data && size > 0)
	{
		uint ranges[5];
		char* cursor = data;
		uint bytes = sizeof(ranges);

		//Ranges
		memcpy(ranges, cursor, bytes);

		numIndices = ranges[0];
		numVertices = ranges[1];
		numNormals = ranges[2];
		numTexCoords = ranges[3];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * numIndices;

		indices = new uint[numIndices];
		memcpy(indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * numVertices * 3;

		vertices = new float[numVertices * 3];
		memcpy(vertices, cursor, bytes);

		//Normals
		if (ranges[2] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * numNormals * 3;

			normals = new float[numNormals * 3];
			memcpy(normals, cursor, bytes);
		}

		//UV's
		if (ranges[3])
		{
			cursor += bytes;
			bytes = sizeof(float) * numTexCoords * 2;

			texCoords = new float[numTexCoords * 2];
			memcpy(texCoords, cursor, bytes);
		}

	}
	else
		_LOG(LOG_ERROR, "Error loading the mesh: '%s'.", realName.c_str());


	RELEASE_ARRAY(data);
}

bool ResourceMesh::loadToMemory()
{
	bool ret = false;

	_LOG(LOG_STD, "Loading mesh to VRAM");

	if (numVertices > 0 && numIndices > 0)
	{
		glGenBuffers(1, (GLuint*)&idVertices);
		glGenBuffers(1, (GLuint*)&idIndices);

		glBindBuffer(GL_ARRAY_BUFFER, idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, indices, GL_STATIC_DRAW);

		if (numNormals > 0)
		{
			glGenBuffers(1, (GLuint*)&idNormals);
			glBindBuffer(GL_ARRAY_BUFFER, idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numNormals * 3, normals, GL_STATIC_DRAW);
		}

		if (numTexCoords> 0)
		{
			glGenBuffers(1, (GLuint*)&idTexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numTexCoords * 2, texCoords, GL_STATIC_DRAW);
		}

		ret = true;
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load to VRAM because there are no vertices or indices. Vertices number: %d, indices number: %d", numVertices, numIndices);
		ret = false;
	}

	++instancesInMemory;

	return ret;
}

void ResourceMesh::clearResMesh()
{
	if(isInMemory())
		removeFromMemory();

	numIndices = 0;
	numVertices = 0;
	numNormals = 0;
	numTexCoords = 0;

	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(texCoords);
}

bool ResourceMesh::removeFromMemory()
{
	if (idIndices > 0) glDeleteBuffers(1, &idIndices);
	if (idVertices > 0) glDeleteBuffers(1, &idVertices);
	if (idNormals > 0) glDeleteBuffers(1, &idNormals);
	if (idTexCoords > 0) glDeleteBuffers(1, &idTexCoords);

	return true;
}