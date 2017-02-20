#ifndef __RESOURCEMESH__
#define __RESOURCEMESH__

#include "Resource.h"
#include "Math.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint UID);
	virtual ~ResourceMesh();

	void LoadMeshResource(const char* fileName, const char* path = nullptr);

	bool LoadInMemory()override;
	bool RemoveFromMemory()override;

	void Save(FileParser& file)override;
	void Load(FileParser& file)override;

	bool LoadInMemoryBuff();

private:

public:
	uint idVertices = 0;
	uint numVertices = 0;
	uint* indices = nullptr;

	uint idIndices = 0;
	uint numIndices = 0;
	float* vertices = nullptr; //!NOTE: remember, the size of the buffer must be the number of vertices * 3

	uint idNormals = 0;
	float* normals = nullptr; //!NOTE: remember, the size of the buffer must be the number of vertices * 3

	uint idTexCoords = 0;
	float* texCoords = nullptr; //!NOTE: remember, the size of the buffer must be the number of vertices * 2

	uint idColors = 0;
	float* colors = nullptr; //!NOTE: remember, the size of the buffer must be the number of vertices * 3

	//TODO: add vertex colors

	AABB aabb;

private:

};

#endif // !__RESOURCEMESH__