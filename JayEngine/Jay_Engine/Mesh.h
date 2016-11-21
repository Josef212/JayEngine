#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"

struct aiMesh;
class ResourceMesh;
class FileParser;

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

	ResourceMesh* createAnEmptyMeshRes();

	void clearMesh();

	bool saveCMP(FileParser* sect);
	bool loadCMP(FileParser* sect);

private:

public:
	ResourceMesh* meshResource = NULL;

	/**Will use a similar system as assimp, each mesh has the id of each texture, 
	the id is the index of the textures array in the material component of the game object. */
	int idTexture = -1; 

	bool renderWireframe = false;
	bool renderNormals = false;

private:
	bool onVRAM = false;
};

#endif // !__MESH_H__