#ifndef __MESH__
#define __MESH__

#include "Component.h"
#include "ComponentResource.h"
#include "Math.h"
#include "Color.h"

class ResourceMesh;
class FileParser;

class Mesh : public Component, public ComponentResource
{
public:
	Mesh(GameObject* gObj);
	virtual ~Mesh();

	void OnStart()override;

	void GetBox(AABB& box)const override;

	void ClearMesh();

	bool SaveCMP(FileParser& sect)const override;
	bool LoadCMP(FileParser* sect)override;

	bool SetResource(UID resUID)override;

private:

public:
	bool renderWireframe = false;
	bool renderNormals = false;

	Color tint = White;

private:
	bool onVRAM = false;
};

#endif // !__MESH__