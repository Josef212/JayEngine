#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint UID);
	virtual ~ResourceMesh();

	void loadMeshResource(const char* fileName, const char* path = NULL);
	bool loadToMemory();
	bool removeFromMemory();

private:
	void clearResMesh();

public:
	uint idVertices = 0;
	uint numVertices = 0;
	uint* indices = NULL;

	uint idIndices = 0;
	uint numIndices = 0;
	float* vertices = NULL; //!NOTE: remember, the size of the buffer must be the number of vertices * 3

	uint idNormals = 0;
	uint numNormals = 0;
	float* normals = NULL; //!NOTE: remember, the size of the buffer must be the number of vertices * 3

	uint idTexCoords = 0;
	uint numTexCoords = 0;
	float* texCoords = NULL; //!NOTE: remember, the size of the buffer must be the number of vertices * 2

	//TODO: add vertex colors

private:

};

#endif // !__RESOURCEMESH_H__