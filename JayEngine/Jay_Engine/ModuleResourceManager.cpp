#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"

#include "FileParser.h"
#include "ModuleFileSystem.h"
#include "RandGen.h"

#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"

//TMP
#include "Timer.h"

#include "Importer.h"
#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"

#include "StringTools.h"


ModuleResourceManager::ModuleResourceManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Resource manager: Creation.");
	name.assign("module_importer");

	//TODO: only if mode editor
	sceneImporter = new ImporterScene();
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

	//TODO: Load and save the resource map.
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

Resource* ModuleResourceManager::createNewResource(ResourceType type, UID forceUID)
{
	Resource* ret = NULL;

	if(forceUID == 0 || getResourceFromUID(forceUID))
		forceUID = getNewUID();

	switch (type)
	{
		case RESOURCE_MESH:
			ret = new ResourceMesh(forceUID);
			break;

		case RESOURCE_TEXTURE:
			ret = new ResourceTexture(forceUID);
			break;

		case RESOURCE_SHADER:
			break;

		case RESOURCE_SCENE:
			ret = new ResourceScene(forceUID);
			break;

		case RESOURCE_MATERIAL:
			break;

		default:
			_LOG(LOG_ERROR, "Invalid resource type.");
			break;
	}

	if (ret)
		resources[forceUID] = ret;

	return ret;
}

ResourceType ModuleResourceManager::getTypeFromExtension(const char* extension)
{
	ResourceType ret = RESOURCE_UNKNOWN;

	if (extension)
	{
		if (strcmp(extension, "wav") == 0)
			ret = RESOURCE_AUDIO;
		else if (strcmp(extension, "ogg") == 0)
			ret = RESOURCE_AUDIO;
		else if (strcmp(extension, "dds") == 0)
			ret = RESOURCE_TEXTURE;
		else if (strcmp(extension, "png") == 0)
			ret = RESOURCE_TEXTURE;
		else if (strcmp(extension, "jpg") == 0)
			ret = RESOURCE_TEXTURE;
		else if (strcmp(extension, "tga") == 0)
			ret = RESOURCE_TEXTURE;
		else if (strcmp(extension, "fbx") == 0)
			ret = RESOURCE_SCENE;
		else if (strcmp(extension, "jayscene") == 0)
			ret = RESOURCE_SCENE;
		else if (strcmp(extension, "jaymesh") == 0)
			ret = RESOURCE_MESH;
		else if (strcmp(extension, "jaymat") == 0)
			ret = RESOURCE_MATERIAL;
	}

	return ret;
}

Resource* ModuleResourceManager::getResourceFromUID(UID uuid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uuid);

	return (it != resources.end()) ? ((*it).second) : (NULL);
}

UID ModuleResourceManager::findResource(const char* fileInAssets)const
{
	std::string file(fileInAssets);
	app->fs->normalizePath(file);

	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->originalFile.compare(file) == 0)
			return it->first;
	}

	return 0;
}

UID ModuleResourceManager::importFile(const char* fileInAssets, bool checkFirst)
{
	UID ret = 0;

	std::string original(fileInAssets);
	std::string path, file, ext;
	app->fs->normalizePath(original);
	app->fs->splitPath(original.c_str(), &path, &file, &ext);
	
	if (checkFirst)
	{
		ret = findResource(file.c_str());
		if (ret != 0)
			return ret;
	}

	ResourceType type = getTypeFromExtension(ext.c_str());

	bool succes = false;
	std::string exportedFile;
	UID resUID = 0;

	switch (type)
	{
		//TODO: Add more cases for new resources type.

	case RESOURCE_TEXTURE:
		succes = textureImporter->import(file.c_str(), exportedFile, resUID);
		break;

	case RESOURCE_SCENE:
	{
		std::string full;
		if (path.empty())
			full.assign(DEFAULT_FB_PATH);
		else
			full.assign(path);
		full.append(file);

		succes = sceneImporter->import(full.c_str(), exportedFile, ext.c_str(), resUID); 
	}
		break;

	default:
		_LOG(LOG_ERROR, "Could not recognize file extension.");
		break;
	}

	//If import has success then create the resource. And assign name and ID info, not all info only needed info to reload resource
	if (succes)
	{
		Resource* res = createNewResource(type, resUID);
		res->originalFile = file;
		std::string eFile;
		app->fs->normalizePath(exportedFile);
		app->fs->splitPath(exportedFile.c_str(), NULL, &eFile);
		res->exportedFile = eFile;
		ret = res->getUID();

		_LOG(LOG_INFO, "Just imported from '%s' to '%s'.", res->getOriginalFile(), res->getExportedFile());
	}
	else
	{
		_LOG(LOG_ERROR, "FAILED importing '%s'.", fileInAssets);
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
		res->removeInstance();

		if (res->countReferences() <= 0)
		{
			//Must remove it from the map and clean the info
			resources.erase(uuid);
			res->removeFromMemory();
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
		for (uint i = 0; i < fbxCount; ++i)
		{
			importFile(fbxs[i].c_str(), true);
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