#ifndef __IMPORTER_MESH__
#define __IMPORTER_MESH__

#include <string>

class Resource;
class ResourceMesh;
struct aiMesh;

class ImporterMesh
{
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	void ImportMesh(const aiMesh* mesh, ResourceMesh* resMesh);
	bool LoadResource(ResourceMesh* resource);
};

#endif // !__IMPORTERMESH_H__