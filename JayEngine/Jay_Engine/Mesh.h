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

	void clearMesh();

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
	uint numTexCoords = 0; //Same texCoords as num of vertices but here will save numVertices*3 because each texCoord has 3 components, actually uv's are 2D but keep the 3rd variable
	float* texCoords = NULL;

	/**Will use a similar system as assimp, each mesh has the id of each texture, 
	the id is the index of the textures array in the material component of the game object. */
	int idTexture = -1; 

	bool renderWireframe = false;
	bool renderNormals = false;

private:
	bool onVRAM = false;
};

#endif // !__MESH_H__