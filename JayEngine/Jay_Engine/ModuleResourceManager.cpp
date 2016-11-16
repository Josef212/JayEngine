#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "ModuleFileSystem.h"
#include "RandGen.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"

//TMP
#include "Timer.h"

#include "Importer.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"


ModuleResourceManager::ModuleResourceManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Resource manager: Creation.");
	name.assign("module_importer");

	//TODO: only if mode editor
	meshImporter = new ImporterMesh();
	materialImporter = new ImporterMaterial();
	//----------------------------- endif
}


ModuleResourceManager::~ModuleResourceManager()
{
	_LOG(LOG_STD, "Resource manager: Destroying.");
}

bool ModuleResourceManager::init(FileParser* conf)
{
	_LOG(LOG_STD, "Resource manager: Init.");
	bool ret = true;
	
	return ret;
}

bool ModuleResourceManager::start()
{
	_LOG(LOG_STD, "Importer: Start.");

	Timer timer;
	timer.Start();
	meshImporter->importFBX("Cube.fbx");
	_LOG(LOG_WARN, "Importing lasted: %dms.", timer.Read());


	Timer timer2;
	timer2.Start();
	ResourceMesh* res = (ResourceMesh*)createNewResource(RESOURCE_MESH);
	_LOG(LOG_WARN, "Importing lasted: %dms.", timer2.Read());

	return true;
}

bool ModuleResourceManager::cleanUp()
{
	_LOG(LOG_STD, "Resource manager: CleanUp.");
	bool ret = true;
	
	return ret;
}

UID ModuleResourceManager::getNewUID()
{
	return app->random->getRandInt();
}

Resource* ModuleResourceManager::createNewResource(ResourceType type)
{
	Resource* ret = NULL;
	UID uuid = getNewUID();

	switch (type)
	{
		case RESOURCE_MESH:
		{
			ret = new ResourceMesh(uuid);
		}
		break;

		case RESOURCE_MATERIAL:
		{
			ret = new ResourceMaterial(uuid);
		}
		break;

		default:
			_LOG(LOG_ERROR, "Invalid resource type.");
		break;
	}

	if (ret)
		resources[uuid] = ret;

	return ret;
}

Resource* ModuleResourceManager::getResourceFromUID(UID uuid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uuid);

	return (it != resources.end()) ? ((*it).second) : (NULL);
}