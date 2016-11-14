#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "ModuleFileSystem.h"
#include "RandGen.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"

#include "Importer.h"
#include "ImporterMesh.h"
#include "ImporterMaterial.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


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

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleResourceManager::start()
{
	_LOG(LOG_STD, "Importer: Start.");

	return true;
}

bool ModuleResourceManager::cleanUp()
{
	_LOG(LOG_STD, "Resource manager: CleanUp.");
	bool ret = true;

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}

uint ModuleResourceManager::getNewUID()
{
	return app->random->getRandInt();
}

Resource* ModuleResourceManager::createNewResource(ResourceTypes type)
{
	Resource* ret = NULL;

	switch (type)
	{
		case RESOURCE_MESH:
		{
			ret = new ResourceMesh(getNewUID());
		}
		break;

		case RESOURCE_MATERIAL:
		{
			ret = new ResourceMaterial(getNewUID());
		}
		break;

		default:
			_LOG(LOG_ERROR, "Invalid resource type.");
		break;
	}

	return ret;
}