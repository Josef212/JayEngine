#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

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

	void importMesh(const aiMesh* mesh, ResourceMesh* resMesh);
	bool loadResource(ResourceMesh* resource);
};

#endif // !__IMPORTERMESH_H__