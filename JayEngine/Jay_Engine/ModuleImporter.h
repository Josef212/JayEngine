#ifndef __MODULEIMPORTER_H__
#define __MODULEIMPORTER_H__

#include "Module.h"

struct Mesh
{
	uint idVertices = 0;
	uint numVertices = 0;
	uint* indices = NULL;

	uint idIndices = 0;
	uint numIndices = 0;
	float* vertices = NULL;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool startEnabled = true);
	~ModuleImporter();

	bool init();

	bool cleanUp();
	void loadFBX(const char* path);
	void drawAllFBX();

public:
	Mesh* meshes = NULL;
	uint meshesNum = 0;
private:
	
};

#endif // !__MODULEIMPORTER_H__