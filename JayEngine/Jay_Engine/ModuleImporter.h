#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include <vector>

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
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool startEnabled = true);
	~ModuleImporter();

	bool init();
	bool start();
	update_status postUpdate(float dt);
	bool cleanUp();
	void loadFBX(const char* path, std::vector<VertexInfo>& vec);
	void drawMeshes(std::vector<VertexInfo> vec);

public:

private:
	std::vector<VertexInfo> meshes;
};

#endif // !__MODULEIMPORTER_H__