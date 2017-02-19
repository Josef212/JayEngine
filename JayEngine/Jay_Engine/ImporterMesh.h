#ifndef __IMPORTER_MESH__
#define __IMPORTER_MESH__

#include "Importer.h"
#include <string>

class Resource;
class ResourceMesh;
struct aiMesh;

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	bool LoadResource(Resource* resource)override;

	bool Import(const aiMesh* mesh, std::string& output, UID& id);
	void GenBuffers(const ResourceMesh* resource);

	bool LoadCube(ResourceMesh* res);
	bool LoadSphere(ResourceMesh* res);
	bool LoadCylinder(ResourceMesh* res);
	bool LoadCone(ResourceMesh* res);
	bool LoadPyramid(ResourceMesh* res);
	bool LoadTorus(ResourceMesh* res);


	//Old
	void ImportMesh(const aiMesh* mesh, ResourceMesh* resMesh);
};

#endif // !__IMPORTERMESH_H__