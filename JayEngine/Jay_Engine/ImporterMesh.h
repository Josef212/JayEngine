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

	void ImportMesh(const aiMesh* mesh, ResourceMesh* resMesh);
	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTERMESH_H__