#ifndef __IMPORTERFBX_H__
#define __IMPORTERFBX_H__

#include "Importer.h"
#include <string>
#include <map>

struct aiNode;
struct aiScene;
class ResourceMesh;
class GameObject;

class ImporterScene :	public Importer
{
public:
	ImporterScene();
	virtual ~ImporterScene();

	bool import(const char* originalFile, std::string& exportedFile, const char* originalFileExtension, UID& resUID);

private:
	void recImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file);
public:

private:
	std::map<int, ResourceMesh*> meshesImported;//Key is the index from scene->meshes
};

#endif // !__IMPORTERFBX_H__