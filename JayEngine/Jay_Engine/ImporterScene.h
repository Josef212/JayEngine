#ifndef __IMPORTERFBX_H__
#define __IMPORTERFBX_H__

#include "Importer.h"
#include <string>
#include <map>

struct aiNode;
struct aiScene;
class FileParser;
class ResourceMesh;
class ResourceTexture;
class GameObject;

class ImporterScene :	public Importer
{
public:
	ImporterScene();
	virtual ~ImporterScene();

	bool import(const char* originalFile, std::string& exportedFile, const char* originalFileExtension, UID& resUID);

	bool importFBX(const char* fullPath, const char* fileName);

private:
	void recImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file);
	GameObject* importFBXRec(aiNode* node, const aiScene* scene, GameObject* parent, const char* originalFBX);

public:

private:
	std::map<int, ResourceMesh*> meshesImported;//Key is the index from scene->meshes //TODO: Do the same for textures? 
	std::map<std::string, ResourceTexture*> texturesImported;
};

#endif // !__IMPORTERFBX_H__