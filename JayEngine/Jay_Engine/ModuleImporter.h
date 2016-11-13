#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"

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

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool startEnabled = true);
	~ModuleImporter();

	bool init(FileParser* conf);
	bool start();
	bool cleanUp();

	void importFBX(const char* path);

public:

private:
};

#endif // !__MODULEIMPORTER_H__