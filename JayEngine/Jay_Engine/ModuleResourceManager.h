#ifndef __MODULERESOURCEMANAGER__
#define __MODULERESOURCEMANAGER__

#include "Module.h"

#include <string>
#include <vector>
#include <map>

class Resource;
class ResourceMesh;
class ResourceTexture;
class ResourceShader;

enum ResourceType;

class ImporterScene;
class ImporterMesh;
class ImporterTexture;
class ImporterShader;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool startEnabled = true);
	~ModuleResourceManager();

	bool Init(FileParser* conf)override;
	bool Start()override;
	bool CleanUp()override;

	/**
		Import a file depending on the format and return its UID.
	*/
	UID ImportFile(const char* fileInAssets, bool checkFirst = false);

	/**
		Load a resource.
	*/
	bool LoadResource(Resource* resource);
	
	/** 
		Search in resources if there is a resource with file name passed, if found return its UID, if not 0;
	*/
	UID FindResourceInAssets(const char* fileInAssets)const;

	/**
		Return resource type from file extension.
	*/
	ResourceType GetTypeFromExtension(const char* extension);

	/**
		Called when a resource is removed, if it was tha last instance will remove from memory if not just rest an instance
	*/
	void OnResourceRemove(Resource* resource);

	UID GetNewUID();

	Resource* CreateNewResource(ResourceType type, UID forceUID = 0);
	Resource* GetResourceFromUID(UID uuid);

	bool AddResource(Resource* res, UID uuid);
	bool RemoveResource(UID uuid);

	bool AddPrefab(const char* originalFile, const char* exportedFile); //Both will be raw file names, path must be added

	void GetResourcesOfType(std::vector<Resource*>& res, ResourceType type)const;

	ResourceShader* GetDefaultShader()const;

private:
	bool AutoImportFBX();
	bool CheckAllPrefabs();

	bool LoadResources();
	bool SaveResources();

	void LoadBasicResources();

public:
	ImporterScene* sceneImporter = NULL;
	ImporterMesh* meshImporter = NULL;
	ImporterTexture* textureImporter = NULL;
	ImporterShader* shaderImporter = NULL;

private:
	std::map<UID, Resource*> resources;

	std::vector<ResourceMesh*> primitives;
	ResourceTexture* checkers = NULL;

	ResourceShader* defaultShader = NULL;




	std::map<const char*, std::string> prefabs;

};

#endif // !__MODULERESOURCEMANAGER__