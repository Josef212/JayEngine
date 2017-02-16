#ifndef __MESH__
#define __MESH__

#include "Component.h"
#include "Math.h"

class ResourceMesh;
class FileParser;

class Mesh : public Component
{
public:
	Mesh(GameObject* gObj, int id);
	virtual ~Mesh();

	void Init()override;
	void Update(float dt)override;
	void CleanUp()override;

	void GetBox(AABB& box)const override;

	void ClearMesh();

	bool SaveCMP(FileParser& sect)override;
	bool LoadCMP(FileParser& sect)override;

	void SetResource(UID resUID)override;

private:

public:
	ResourceMesh* meshResource = NULL;

	bool renderWireframe = false;
	bool renderNormals = false;

private:
	bool onVRAM = false;
};

#endif // !__MESH__