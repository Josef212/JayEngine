#ifndef __RESOURCEMESH_H__
#define __RESOURCEMESH_H__

#include "Resource.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint UID);
	virtual ~ResourceMesh();
};

#endif // !__RESOURCEMESH_H__


/*struct VertexInfo
{
uint idVertices = 0;
uint numVertices = 0;
uint* indices = NULL;

uint idIndices = 0;
uint numIndices = 0;
float* vertices = NULL;

uint idNormals = 0;
uint numNormals = 0; //Each vertex has a normal vec and this var is the size of normals vec(numVertices*3)
float* normals = NULL;

uint idTexCoords = 0;
uint numTexCoords = 0;
float* texCoords = NULL;
};*/