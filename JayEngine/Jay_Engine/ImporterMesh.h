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

	void importMesh(aiMesh* mesh, ResourceMesh* resMesh);

};

#endif // !__IMPORTERMESH_H__