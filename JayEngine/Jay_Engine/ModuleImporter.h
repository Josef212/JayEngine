#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"
#include <vector>

struct Mesh
{
	uint idVertices = 0;
	uint numVertices = 0;
	uint* indices = NULL;

	uint idIndices = 0;
	uint numIndices = 0;
	float* vertices = NULL;

	uint numFaces = 0;

	uint numNormals = 0;
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
	void loadFBX(const char* path, std::vector<Mesh>& vec);
	void drawMeshes(std::vector<Mesh> vec);

public:

private:
	std::vector<Mesh> meshes;
};

#endif // !__MODULEIMPORTER_H__