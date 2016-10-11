#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"

struct aiMesh;

class Mesh : public Component
{
public:
	Mesh(GameObject* gObj, int id);
	virtual ~Mesh();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

	bool loadMesh(aiMesh* mesh, bool loadToRAM = true);
	bool loadToOpenGl();

private:

public:
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
	uint numTexCoords = 0; //Same texCoords as num of vertices but here will save numVertices*2 because each texCoord has 2 components
	float* texCoords = NULL;

private:
	bool onVRAM = false;
};

#endif // !__MESH_H__