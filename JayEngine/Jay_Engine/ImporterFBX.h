#ifndef __IMPORTERFBX_H__
#define __IMPORTERFBX_H__

#include "Importer.h"

struct aiNode;
struct aiScene;
class FileParser;

class GameObject;

class ImporterFBX :	public Importer
{
public:
	ImporterFBX();
	virtual ~ImporterFBX();

	bool importFBX(const char* fullPath, const char* fileName);

private:
	GameObject* importFBXRec(aiNode* node, const aiScene* scene, GameObject* parent, const char* originalFBX);
};

#endif // !__IMPORTERFBX_H__