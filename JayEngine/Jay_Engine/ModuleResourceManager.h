#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

class Mesh;

struct VertexInfo
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
};

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool startEnabled = true);
	~ModuleResourceManager();

	bool init(FileParser* conf);
	bool start();
	bool cleanUp();

public:

private:
};

#endif // !__MODULERESOURCEMANAGER_H__