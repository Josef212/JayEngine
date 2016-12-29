#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"
#include "Math.h"

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

	void getBox(AABB& box)const;

	ResourceMesh* createAnEmptyMeshRes();

	void clearMesh();

	bool saveCMP(FileParser& sect);
	bool loadCMP(FileParser& sect);

	void setResource(UID resUID);

private:

public:
	ResourceMesh* meshResource = NULL;

	bool renderWireframe = false;
	bool renderNormals = false;

private:
	bool onVRAM = false;
};

#endif // !__MESH_H__