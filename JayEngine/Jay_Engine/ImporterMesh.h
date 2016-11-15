#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

#include "Importer.h"

class Resource;
class ResourceMesh;
struct aiMesh;

class ImporterMesh : public Importer
{
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	void importFBX(const char* fbxName);
	void importMesh(aiMesh* mesh, ResourceMesh* resMesh);
	void loadMesh(const char* fileName, ResourceMesh* resMesh);

};

#endif // !__IMPORTERMESH_H__