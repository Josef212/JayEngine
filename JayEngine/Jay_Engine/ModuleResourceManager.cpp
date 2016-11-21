#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "ModuleFileSystem.h"
#include "RandGen.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

//TMP
#include "Timer.h"

#include "Importer.h"
#include "ImporterFBX.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"


ModuleResourceManager::ModuleResourceManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Resource manager: Creation.");
	name.assign("module_importer");

	//TODO: only if mode editor
	fbxImporter = new ImporterFBX();
	meshImporter = new ImporterMesh();
	textureImporter = new ImporterTexture();
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
	std::string name;
	timer.Start();
	//textureImporter->importTexture("Lenna.png", name);
	_LOG(LOG_WARN, "Importing lasted: %dms.", timer.Read());


	Timer timer2;
	timer2.Start();
	ResourceMesh* res = (ResourceMesh*)createNewResource(RESOURCE_MESH);
	//_LOG(LOG_WARN, "Importing lasted: %dms.", timer2.Read());



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

		case RESOURCE_TEXTURE:
		{
			ret = new ResourceTexture(uuid);
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

bool ModuleResourceManager::importFBX(const char* name, const char* path)
{
	bool ret = true;

	if (name)
	{
		char fullPath[64];
		if (path)
			strcpy_s(fullPath, 64, path);
		else
			strcpy_s(fullPath, 64, DEFAULT_FB_PATH);

		strcat_s(fullPath, 64, "/");
		strcat_s(fullPath, 64, name);

		ret = fbxImporter->importFBX(fullPath, name);
	}
	else
	{
		_LOG(LOG_ERROR, "Invalid fbx name!");
		ret = false;
	}

	return ret;
}