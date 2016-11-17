#ifndef __MODULERESOURCEMANAGER_H__
#define __MODULERESOURCEMANAGER_H__

#include "Module.h"

#include <map>

class Resource;
class ResourceMesh;
class ResourceTexture;
enum ResourceType;

class Importer;
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

	UID getNewUID();

	Resource* createNewResource(ResourceType type);
	Resource* getResourceFromUID(UID uuid);

private:

public:
	ImporterMesh* meshImporter = NULL;
	ImporterTexture* textureImporter = NULL;

private:
	std::map<UID, Resource*> resources;

};

#endif // !__MODULERESOURCEMANAGER_H__