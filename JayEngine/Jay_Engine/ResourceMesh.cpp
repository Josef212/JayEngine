#include "ResourceMesh.h"

#include "OpenGL.h"

ResourceMesh::ResourceMesh(UID uuid) : Resource(uuid)
{
	resType = RESOURCE_MESH;
}


ResourceMesh::~ResourceMesh()
{
	clearResMesh();
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