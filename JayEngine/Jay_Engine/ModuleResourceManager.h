#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

#include <map>

class Resource;
class ResourceMesh;
class ResourceMaterial;
enum ResourceTypes;

class Importer;
class ImporterMesh;
class ImporterMaterial;

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool startEnabled = true);
	~ModuleResourceManager();

	bool init(FileParser* conf);
	bool start();
	bool cleanUp();

	uint getNewUID();

	Resource* createNewResource(ResourceTypes type);

private:

public:
	ImporterMesh* meshImporter = NULL;
	ImporterMaterial* materialImporter = NULL;

private:
	std::map<uint, Resource*> resources;

};

#endif // !__MODULERESOURCEMANAGER_H__