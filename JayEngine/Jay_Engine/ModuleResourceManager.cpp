#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "FileParser.h"
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

#include "StringTools.h"


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

	//checkAllPrefabs();
	autoImportFBX();

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

		std::string out;
		removeExtension(name, out);
		out.append(".json");

		//--------------------------

		ret = fbxImporter->importFBX(fullPath, out.c_str());

		if (ret)
		{
			addPrefab(name, out.c_str());
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

bool ModuleResourceManager::removeResource(UID uuid)
{
	bool ret = false;

	Resource* res = getResourceFromUID(uuid);

	if (res)
	{
		if (res->countReferences() <= 0)
		{
			//Must remove it from the map and clean the info
			resources.erase(uuid);
			RELEASE(res);
		}
	}

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

/** 
	On engine start check all fbx and import them if havent been imported yet.
	//TODO: This might be done every certain time in order to check if new files have been dropped directly to the folder.
*/
bool ModuleResourceManager::autoImportFBX()
{
	bool ret = true;

	std::vector<std::string> fbxs;
	uint fbxCount = app->fs->getFilesOnDir(DEFAULT_FB_PATH, fbxs);

	if (fbxCount > 0)
	{
		std::vector<std::string> prefabs;
		uint prefabsCount = app->fs->getFilesOnDir(DEFAULT_PREF_SAVE_PATHS, prefabs);

		if (prefabsCount > 0) /** Must check individually all fbx and prefabs. */
		{
			for (uint i = 0; i < fbxCount; ++i) //Here will need to generate the file name should have after the importation in order to search it in prefabs vector.
			{
				std::string out;
				removeExtension(fbxs[i].c_str(), out);
				out.append(".json");
				//--------------------------

				//Now we have the name of the file after importation so search it on prefabs vector, if found add it to prefabs map if not import it.
				
				std::vector<std::string>::iterator it = std::find(prefabs.begin(), prefabs.end(), out);
				if (it != prefabs.end())
				{
					addPrefab(fbxs[i].c_str(), out.c_str());
				}
				else
				{
					importFBX(fbxs[i].c_str());
				}
			}
		}
		else /** Directly import all fbx because none have been imported. */
		{
			for (uint i = 0; i < fbxCount; ++i)
			{
				importFBX(fbxs[i].c_str()); //As this method automatically creates the exported file name and adds it to the map just do this.
			}
		}
	}

	return ret;
}

bool ModuleResourceManager::checkAllPrefabs() //Is this really important??? //TODO: Instead of doing this why not serialitzate the prefabs map and check files from there??
{
	bool ret = true;

	std::vector<std::string> prefabs;
	uint count = app->fs->getFilesOnDir(DEFAULT_PREF_SAVE_PATHS, prefabs);

	/**
		If there are files in this directory lets check all and see if all the resources they need exist.
	*/
	if (count > 0)
	{
		for (uint i = 0; i < count; ++i)
		{
			//TODO: search for fbx original file, if it no longer exist delete all the files involved in this prefab.

			std::string fileName(DEFAULT_PREF_SAVE_PATHS + prefabs[i]);
			char* buffer = NULL;
			uint size = app->fs->load(fileName.c_str(), &buffer);

			if (size > 0 && buffer)
			{
				FileParser file(buffer);
				int goCount = file.getArraySize("GameObjects");
				for (uint j = 0; j < goCount; ++j)
				{
					FileParser go = file.getArray("GameObjects", j);
					int cmpCount = file.getArraySize("Components");
					for (uint t = 0; t < cmpCount; ++t)
					{
						FileParser cmp(file.getArray("Components", t));
						//TODO: Check here all resources.
					}
				}
			}

		}
	}

	return ret;
}