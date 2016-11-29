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
	//_LOG(LOG_WARN, "Importing lasted: %dms.", timer.Read());


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


/** 
	-Name: Must be the fbx name with extension.			Ex: example.fbx
	-Path: Must be the realtive path of the file.		Ex: Data/Assets/fbx


	-Exported name: Will be the final exported file.		Ex: example.json
	-Fullpath: Will be the full path of original file.		Ex: Data/Assets/fbx/example.fbx
*/
bool ModuleResourceManager::importFBX(const char* name, const char* path)
{
	bool ret = true;

	if (!name)
	{
		_LOG(LOG_ERROR, "Invalid fbx name!");
		ret = false;
	}

	std::map<const char*, std::string>::iterator it = prefabs.find(name);

	if (ret && it == prefabs.end()) //TODO: Instead of not loading the fbx load it and update all resources. Also check if the resources of all meshes and textures are in the library
	{
		char fullPath[64];
		if (path)
			strcpy_s(fullPath, 64, path);
		else
			strcpy_s(fullPath, 64, DEFAULT_FB_PATH);

		strcat_s(fullPath, 64, "/");
		strcat_s(fullPath, 64, name);


		char exported[64];
		strcpy_s(exported, 64, name);
		//Clean the extension--------------
		uint s = strlen(name);
		if (s > 0)
		{
			char* it = exported;
			it += s;

			while (*it != '.')
			{
				--it;
				--s;
			}

			if(s < strlen(exported))
				exported[s] = '\0';

			strcat_s(exported, 64, ".json");
		}

		//--------------------------

		ret = fbxImporter->importFBX(fullPath, exported);

		if (ret)
		{
			addPrefab(name, exported);
		}
	}
	else
	{
		_LOG(LOG_WARN, "FBX already imported into prefab. Original: %s. Exported: %s.", name, (*it).second.c_str());
		ret = false;
	}

	return ret;
}

bool ModuleResourceManager::addResource(Resource* res, UID uuid)
{
	bool ret = true;

	if (res && uuid != 0)
	{
		resources.insert(std::pair<UID, Resource*>(uuid, res)); //TODO: Check if resource already exist?
	}
	else
		ret = false;

	return ret;
}

bool ModuleResourceManager::addPrefab(const char* originalFile, const char* exportedFile)
{
	bool ret = true;

	std::map<const char*, std::string>::iterator it = prefabs.find(originalFile);

	if (it != prefabs.end())
	{
		_LOG(LOG_ERROR, "Prefab '%s' already imported to '%s'.", originalFile, (*it).second.c_str());
		ret = false;
	}
	else
	{
		prefabs.insert(std::pair<const char*, std::string>(originalFile, exportedFile));
		_LOG(LOG_INFO, "New prefab exported. Original: %s. Exported: %s.", originalFile, exportedFile);
	}

	return ret;
}