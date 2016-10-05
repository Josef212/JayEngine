#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"

class Mesh : public Component
{
public:
	Mesh(GameObject* gObj);
	virtual ~Mesh();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

private:
	void loadToOpenGl();

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
};

#endif // !__MESH_H__