#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

#include <string>
#include <vector>
#include <map>

class Resource;
class ResourceMesh;
class ResourceTexture;
enum ResourceType;

class Importer;
class ImporterScene;
class ImporterMesh;
class ImporterTexture;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool startEnabled = true);
	~ModuleResourceManager();

	bool init(FileParser* conf);
	bool start();
	bool cleanUp();

	/**
		Import a file depending on the format and return its UID.
	*/
	UID importFile(const char* fileInAssets, bool checkFirst = false);
	
	/** 
		Search in resources if there is a resource with file name passed, if found return its UID, if not 0;
	*/
	UID findResource(const char* fileInAssets)const;

	/**
		Return resource type from file extension.
	*/
	ResourceType getTypeFromExtension(const char* extension);

	UID getNewUID();

	Resource* createNewResource(ResourceType type, UID forceUID = 0);
	Resource* getResourceFromUID(UID uuid);

	bool addResource(Resource* res, UID uuid);
	bool removeResource(UID uuid);

	bool addPrefab(const char* originalFile, const char* exportedFile); //Both will be raw file names, path must be added

	void getResourcesOfType(std::vector<Resource*>& res, ResourceType type)const;

private:
	bool autoImportFBX();
	bool checkAllPrefabs();

	bool loadResources();
	bool saveResources();

public:
	ImporterScene* sceneImporter = NULL;
	ImporterMesh* meshImporter = NULL;
	ImporterTexture* textureImporter = NULL;

private:
	std::map<UID, Resource*> resources;
	std::map<const char*, std::string> prefabs;

};

#endif // !__MODULERESOURCEMANAGER_H__