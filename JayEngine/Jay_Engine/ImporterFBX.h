#ifndef __IMPORTERFBX_H__
#define __IMPORTERFBX_H__

#include "Importer.h"

struct aiNode;
struct aiScene;
class FileParser;

class ImporterFBX :	public Importer
{
public:
	ImporterFBX();
	virtual ~ImporterFBX();

	bool importFBX(const char* fullPath, const char* fileName);

private:
	void importFBXRec(aiNode* node, const aiScene* scene, UID parent, FileParser* file);
};

#endif // !__IMPORTERFBX_H__