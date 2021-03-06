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
#include "ResourceShader.h"

//TMP
#include "Timer.h"

#include "ImporterScene.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterShader.h"

#include "StringTools.h"

#define RESERVED_RESOURCES 7 //From 1 to 7. Cube-Sphere-Cylinder-Cone-Pyramid-Torus-Chequers


ModuleResourceManager::ModuleResourceManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Resource manager: Creation.");
	name.assign("module_importer");


	sceneImporter = new ImporterScene();
	meshImporter = new ImporterMesh();
	textureImporter = new ImporterTexture();
	shaderImporter = new ImporterShader();
}


ModuleResourceManager::~ModuleResourceManager()
{
	_LOG(LOG_STD, "Resource manager: Destroying.");

	if (sceneImporter) RELEASE(sceneImporter);
	if (meshImporter) RELEASE(meshImporter);
	if (textureImporter) RELEASE(textureImporter);
	if (shaderImporter) RELEASE(shaderImporter);
}

bool ModuleResourceManager::Init(FileParser* conf)
{
	_LOG(LOG_STD, "Resource manager: Init.");
	bool ret = true;
	
	return ret;
}

bool ModuleResourceManager::Start()
{
	_LOG(LOG_STD, "Importer: Start.");

	LoadResources();
	LoadBasicResources();
	//AutoImportFBX();

	ImportFile("Cube.fbx");
	ImportFile("Lenna.png");

	//ResourceScene tmp(1676680638);
	//tmp.exportedFile = "1676680638.jayscene";
	//LoadResource(&tmp);
	
	return true;
}

bool ModuleResourceManager::CleanUp()
{
	_LOG(LOG_STD, "Resource manager: CleanUp.");
	bool ret = true;

	SaveResources();

	//TODO: Make sure all resources are free

	if (defaultShader > 0)
		shaderImporter->FreeShader(defaultShader->shaderID);
	
	return ret;
}

UID ModuleResourceManager::GetNewUID()
{
	return app->random->GetRandInt();
}

Resource* ModuleResourceManager::CreateNewResource(ResourceType type, UID forceUID)
{
	Resource* ret = nullptr;

	if(forceUID == 0 || GetResourceFromUID(forceUID))
		forceUID = GetNewUID();

	switch (type)
	{
		case RESOURCE_MESH:
			ret = new ResourceMesh(forceUID);
			break;

		case RESOURCE_TEXTURE:
			ret = new ResourceTexture(forceUID);
			break;

		case RESOURCE_SHADER:
			ret = new ResourceShader(forceUID);
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

ResourceType ModuleResourceManager::GetTypeFromExtension(const char* extension)
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

Resource* ModuleResourceManager::GetResourceFromUID(UID uuid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uuid);

	return (it != resources.end()) ? ((*it).second) : (nullptr);
}

UID ModuleResourceManager::FindResourceInAssets(const char* fileInAssets)const
{
	std::string file(fileInAssets);
	app->fs->NormalizePath(file);

	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->originalFile.compare(file) == 0)
			return it->first;
	}

	return 0;
}

UID ModuleResourceManager::ImportFile(const char* fileInAssets, bool checkFirst)
{
	UID ret = 0;

	std::string original(fileInAssets);
	std::string path, file, ext;
	app->fs->NormalizePath(original);
	app->fs->SplitPath(original.c_str(), &path, &file, &ext);
	
	if (checkFirst)
	{
		ret = FindResourceInAssets(file.c_str());
		if (ret != 0)
			return ret;
	}

	ResourceType type = GetTypeFromExtension(ext.c_str());

	bool succes = false;
	std::string exportedFile;
	UID resUID = 0;

	switch (type)
	{
		//TODO: Add more cases for new resources type.

	case RESOURCE_TEXTURE:
		succes = textureImporter->Import(file.c_str(), path.empty() ? nullptr : path.c_str() , exportedFile, resUID); //If there is a path, import the texture from there, if not just pass null to easily control that.
		break;

	case RESOURCE_SCENE:
	{
		std::string full;
		if (path.empty())
			full.assign(PATH_ASSETS_MODEL);
		else
			full.assign(path);
		full.append(file);

		succes = sceneImporter->Import(full.c_str(), exportedFile, ext.c_str(), resUID);
	}
		break;

	default:
		_LOG(LOG_ERROR, "Could not recognize file extension.");
		break;
	}

	//If import has success then create the resource. And assign name and ID info, not all info only needed info to reload resource
	if (succes)
	{
		Resource* res = CreateNewResource(type, resUID);
		res->originalFile = file;
		std::string eFile;
		app->fs->NormalizePath(exportedFile);
		app->fs->SplitPath(exportedFile.c_str(), nullptr, &eFile);
		res->exportedFile = eFile;
		ret = res->GetUID();

		_LOG(LOG_INFO, "Just imported from '%s' to '%s'.", res->originalFile.c_str(), res->exportedFile.c_str());
	}
	else
	{
		_LOG(LOG_ERROR, "FAILED importing '%s'.", fileInAssets);
	}

	return ret;
}

UID ModuleResourceManager::ImportBuffer(const void* buffer, uint size, ResourceType type, const char* sourceFile)
{
	UID ret = 0;

	if (!buffer)
		return ret;

	bool succes = false;
	std::string output;
	UID resUID = 0;

	switch (type)
	{
	case RESOURCE_MESH:
		succes = meshImporter->Import((const aiMesh*)buffer, output, resUID);
		break;
	case RESOURCE_TEXTURE:
		textureImporter->ImportBuf(buffer, size, output, resUID);
		break;
	case RESOURCE_SHADER:
		break;
	case RESOURCE_SCENE:
		break;
	}

	if (succes && resUID != 0)
	{
		Resource* res = CreateNewResource(type, resUID);
		if (sourceFile)
		{
			res->originalFile = sourceFile;
			app->fs->NormalizePath(res->originalFile);
		}
		res->exportedFile = output;
		ret = resUID;
		_LOG(LOG_INFO, "Imported successfully from buffer '%s' to '%s'.", res->originalFile.c_str(), res->exportedFile.c_str());
	}
	else
	{
		_LOG(LOG_ERROR, "Could not import the resource from buffer.");
	}

	return ret;
}

bool ModuleResourceManager::LoadResource(Resource* resource)
{
	bool ret = false;

	if (!resource)
		return ret;

	switch (resource->GetType())
	{
	case ResourceType::RESOURCE_MESH:
		ret = meshImporter->LoadResource((ResourceMesh*)resource);
		break;

	case ResourceType::RESOURCE_TEXTURE:
		ret = textureImporter->LoadResource((ResourceTexture*)resource);
		break;

	case ResourceType::RESOURCE_SCENE:
		ret = sceneImporter->LoadResource((ResourceScene*)resource);
		break;

	case ResourceType::RESOURCE_SHADER:

		break;

	case ResourceType::RESOURCE_AUDIO:

		break;

	case ResourceType::RESOURCE_MATERIAL:

		break;

	default:
		break;
	}

	if(ret)
		resource->AddInstance();

	return ret;
}

/** 
	On engine start check all fbx and import them if havent been imported yet.
	//TODO: This might be done every certain time in order to check if new files have been dropped directly to the folder.
*/
bool ModuleResourceManager::AutoImportFBX()
{
	bool ret = true;

	std::vector<std::string> fbxs;
	uint fbxCount = app->fs->GetFilesOnDir(PATH_ASSETS_MODEL, fbxs);

	if (fbxCount > 0)
	{
		for (uint i = 0; i < fbxCount; ++i)
		{
			ImportFile(fbxs[i].c_str(), true);
		}
	}

	return ret;
}

bool ModuleResourceManager::LoadResources()
{
	//Might change this if primitives added
	bool ret = false;

	std::string path(PATH_SETTINGS);
	path.append("resources.json");
	char* buffer = nullptr;
	uint size = app->fs->Load(path.c_str(), &buffer);

	if (buffer && size > 0) 
	{
		FileParser file(buffer);

		int count = file.GetArraySize("Resources");
		for (uint i = 0; i < count; ++i)
		{
			FileParser res(file.GetArray("Resources", i));
			ResourceType type = (ResourceType)res.GetInt("type", RESOURCE_UNKNOWN);
			UID uid = res.GetInt("UID", 0);

			if (GetResourceFromUID(uid)) //If a resource with given UID is found jus continue.
				continue;

			Resource* r = CreateNewResource(type, uid);
			r->Load(res);
		}

		ret = true;
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

bool ModuleResourceManager::SaveResources()
{
	bool ret = false;

	FileParser save;

	save.AddArray("Resources");

	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->first > RESERVED_RESOURCES)
		{
			FileParser res;
			it->second->Save(res);
			res.AddArrayEntry(res);
		}
	}

	char* buffer = nullptr;
	uint size = save.WriteJson(&buffer, false); //TODO: Fast write

	std::string path(PATH_SETTINGS);
	path.append("resources.json");

	if (app->fs->Save(path.c_str(), buffer, size) == size)
		ret = true;

	RELEASE_ARRAY(buffer);

	return ret;
}

void ModuleResourceManager::GetResourcesOfType(std::vector<Resource*>& res, ResourceType type)const
{
	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (it->second->GetType() == type)
			res.push_back(it->second);
	}
}

void ModuleResourceManager::LoadBasicResources()
{
	checkers = (ResourceTexture*)CreateNewResource(RESOURCE_TEXTURE, 1);
	textureImporter->LoadChequers(checkers);
	checkers->AddInstance();

	ResourceMesh* cube = (ResourceMesh*)CreateNewResource(RESOURCE_MESH, 2);
	meshImporter->LoadCube(cube);
	cube->AddInstance();
	primitives.push_back(cube);

	ResourceMesh* sphere = (ResourceMesh*)CreateNewResource(RESOURCE_MESH, 3);
	meshImporter->LoadSphere(sphere);
	sphere->AddInstance();
	primitives.push_back(sphere);

	//TODO: Load more primitives and release them on stop

	defaultShader = new ResourceShader(0);
	shaderImporter->LoadDefaultShader(defaultShader);
}

ResourceShader* ModuleResourceManager::GetDefaultShader()const
{
	return defaultShader;
}















bool ModuleResourceManager::CheckAllPrefabs() //Is this really important??? //TODO: Instead of doing this why not serialitzate the prefabs map and check files from there??
{
	bool ret = true;

	std::vector<std::string> prefabs;
	uint count = app->fs->GetFilesOnDir(PATH_LIBRARY_PREFABS, prefabs);

	/**
		If there are files in this directory lets check all and see if all the resources they need exist.
	*/
	if (count > 0)
	{
		for (uint i = 0; i < count; ++i)
		{
			//TODO: search for fbx original file, if it no longer exist delete all the files involved in this prefab.

			std::string fileName(PATH_LIBRARY_PREFABS + prefabs[i]);
			char* buffer = nullptr;
			uint size = app->fs->Load(fileName.c_str(), &buffer);

			if (size > 0 && buffer)
			{
				FileParser file(buffer);
				int goCount = file.GetArraySize("GameObjects");
				for (uint j = 0; j < goCount; ++j)
				{
					FileParser go = file.GetArray("GameObjects", j);
					int cmpCount = file.GetArraySize("Components");
					for (uint t = 0; t < cmpCount; ++t)
					{
						FileParser cmp(file.GetArray("Components", t));
						//TODO: Check here all resources.
					}
				}
			}

		}
	}

	return ret;
}