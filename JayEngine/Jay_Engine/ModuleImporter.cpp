#include "Application.h"
#include "Globals.h"
#include "ModuleImporter.h"

#include "ModuleFileSystem.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Importer: Creation.");
	name.assign("module_importer");
}


ModuleImporter::~ModuleImporter()
{
	_LOG(LOG_STD, "Importer: Destroying.");
}

bool ModuleImporter::init(FileParser* conf)
{
	_LOG(LOG_STD, "Importer: Init.");
	bool ret = true;

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleImporter::start()
{
	_LOG(LOG_STD, "Importer: Start.");

	return true;
}

bool ModuleImporter::cleanUp()
{
	_LOG(LOG_STD, "Importer: Init.");
	bool ret = true;

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}