#ifndef __IMPORTERFBX_H__
#define __IMPORTERFBX_H__

#include "Importer.h"
#include <map>

struct aiNode;
struct aiScene;
class FileParser;
class ResourceMesh;
class GameObject;

class ImporterFBX :	public Importer
{
public:
	ImporterFBX();
	virtual ~ImporterFBX();

	bool importFBX(const char* fullPath, const char* fileName);

private:
	GameObject* importFBXRec(aiNode* node, const aiScene* scene, GameObject* parent, const char* originalFBX);

public:

private:
	std::map<int, ResourceMesh*> meshesImported;//Key is the index from scene->meshes //TODO: Do the same for textures? 
};

#endif // !__IMPORTERFBX_H__