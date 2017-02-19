#ifndef __IMPORTER_SCENE__
#define __IMPORTER_SCENE__

#include "Importer.h"
#include <string>
#include <map>

struct aiNode;
struct aiScene;
class Resource;
class ResourceMesh;
class ResourceScene;
class GameObject;

class ImporterScene : public Importer
{
public:
	ImporterScene();
	virtual ~ImporterScene();

	bool Import(const char* originalFile, std::string& exportedFile, const char* originalFileExtension, UID& resUID);

	bool LoadResource(Resource* resource)override;

private:
	void RecImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file);
public:

private:
	std::map<int, ResourceMesh*> meshesImported;//Key is the index from scene->meshes
};

#endif // !__IMPORTER_SCENE__