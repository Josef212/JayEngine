#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "ModuleFileSystem.h"

#include <string>
//TMP
#include "Timer.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleResourceManager::ModuleResourceManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Importer: Creation.");
	name.assign("module_importer");
}


ModuleResourceManager::~ModuleResourceManager()
{
	_LOG(LOG_STD, "Importer: Destroying.");
}

bool ModuleResourceManager::init(FileParser* conf)
{
	_LOG(LOG_STD, "Importer: Init.");
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
	_LOG(LOG_STD, "Importer: Init.");
	bool ret = true;

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}
