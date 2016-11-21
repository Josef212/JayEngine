#include "Application.h"
#include "ImporterFBX.h"

#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ImporterFBX::ImporterFBX()
{
	_LOG(LOG_STD, "Creating a fbx importer.");

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterFBX::~ImporterFBX()
{
	//Stop log stream
	aiDetachAllLogStreams();
}

bool ImporterFBX::importFBX(const char* fullPath, const char* fileName)
{
	bool ret = true;

	char* buffer;
	uint size = app->fs->load(fullPath, &buffer);

	const aiScene* scene = NULL;

	if (buffer && size > 0)
	{
		scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		_LOG(LOG_ERROR, "Error loading fbx: %s.", fullPath);
		ret = false;
	}

	if (scene && scene->HasMeshes())
	{
		FileParser file;
		_LOG(LOG_STD, "Loading fbx: %s.", fullPath);

		importFBXRec(scene->mRootNode, scene, 0, &file);

		//Saving the file

		char* buf;
		uint jSize = file.writeJson(&buffer, false);

		char jFileName[128];
		strcpy_s(jFileName, 128, DEFAULT_SCENE_SAVE_PATH);
		strcat_s(jFileName, 128, fileName); //TODO!!: Clean file name
		strcat_s(jFileName, 128, ".json");

		if (app->fs->save(jFileName, buffer, jSize) != jSize)
		{
			_LOG(LOG_ERROR, "Error saving json file: %s.", );
		}

		aiReleaseImport(scene);
		RELEASE_ARRAY(buffer);
	}
	else
	{
		_LOG(LOG_ERROR, "Error loading fbx: %s.", fullPath);
		ret = false;
	}

	return ret;
}


void ImporterFBX::importFBXRec(aiNode* node, const aiScene* scene, UID parent, FileParser* file)
{
	if (parent == 0) //Root of fbx
	{

	}
	else
	{
		//Serch in json a section of the parent object with UID and add array entry with this child
	}
}