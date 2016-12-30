#include "Application.h"
#include "ImporterScene.h"

#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"

#include "ImporterMesh.h"
#include "ImporterTexture.h"

#include "ModuleGOManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"

#include <string>

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ImporterScene::ImporterScene()
{
	_LOG(LOG_STD, "Creating a fbx importer.");

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterScene::~ImporterScene()
{
	//Stop log stream
	aiDetachAllLogStreams();
}


bool ImporterScene::import(const char* originalFile, std::string& exportedFile, const char* originalFileExtension, UID& resUID)
{
	bool ret = false;

	if (!originalFile || !originalFileExtension)
		return ret;

	char* buffer = NULL;
	uint size = app->fs->load(originalFile, &buffer);

	const aiScene* scene = NULL;

	if (buffer && size > 0)
	{
		scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, originalFileExtension);
		RELEASE_ARRAY(buffer);
	}
	else
	{
		_LOG(LOG_ERROR, "Error loading fbx: %s.", originalFile);
		return ret;
	}

	if (scene && scene->HasMeshes())
	{
		std::string path, file;
		app->fs->splitPath(originalFile, &path, &file);

		meshesImported.clear();

		GameObject* go = app->goManager->createGameObject();
		go->setName(file.c_str());

		//Recursive create game objects.
		recImport(scene, scene->mRootNode, go, path, file);

		//TODO: Process bones.

		//TODO: Process animations.

		aiReleaseImport(scene);

		//Serialize Game Objects recursively.
		FileParser save;
		save.addArray("GameObjects");

		for (uint i = 0; i < go->childrens.size(); ++i)
			if (go->childrens[i])
				go->childrens[i]->saveGO(save);

		resUID = app->resourceManager->getNewUID();

		char name[128];
		sprintf_s(name, 128, "%d%s", resUID, SCENE_EXTENSION);
		exportedFile.assign(name);
		char fullPath[256];
		sprintf_s(fullPath, 256, "%s%s", DEFAULT_PREF_SAVE_PATHS, name);

		char* buf = NULL;
		uint _size = save.writeJson(&buf, false); //TODO: When finish testing change to fast mode.

		if (app->fs->save(fullPath, buf, _size) == _size)
			ret = true;

		RELEASE_ARRAY(buf);

		go->remove();
	}

	return ret;
}

void ImporterScene::recImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file)
{
	static std::string name;
	name = (node->mName.length > 0) ? node->mName.C_Str() : "unnamed";

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scl(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	GameObject* go = parent->addChild();
	go->setName(name.c_str());

	Transform* trans = go->transform;

	trans->setLocalPosition(pos);
	trans->setLocalScale(scl);
	trans->setLocalRotation(rot);

	//TODO: Load meta data

	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		int meshIndex = node->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[meshIndex];

		GameObject* childGO = NULL;

		if (node->mNumMeshes > 1)
		{
			name = mesh->mName.C_Str();
			if (name.length() == 0)
			{
				name = node->mName.C_Str();
				name += "_submesh";
			}
			childGO = go->addChild();
			childGO->setName(name.c_str());
		}
		else
			childGO = go;

		//TODO: Adapt this to material resource.

		if (scene->HasMaterials())
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE); //TODO: Someday add other textures as normal maps etc

			Material* cMat = (Material*)childGO->addComponent(ComponentType::MATERIAL);

			aiColor4D col;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			cMat->color.Set(col.r, col.g, col.b, col.a);

			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			if (str.length > 0)
			{
				//Need to import that texture
				std::string texPath(str.C_Str());
				app->fs->normalizePath(texPath);

				//If exist import it. Might check if i should change the path before.
				std::string texFile;
				app->fs->splitPath(texPath.c_str(), NULL, &texFile);
				
				cMat->textureResource = (ResourceTexture*)app->resourceManager->getResourceFromUID(app->resourceManager->importFile(texFile.c_str(), true));
				cMat->textureResource->addInstance();
			}

			//TODO:Check assimp for embedded textures... For now will normally import textures.
			//Add texture
		}


		//Add mesh
		Mesh* cmesh = (Mesh*)childGO->addComponent(ComponentType::MESH);
		//Will track of meshes already loaded in order to not import twice same meshes
		std::map<int, ResourceMesh*>::iterator tmp = meshesImported.find(meshIndex);
		if (tmp != meshesImported.end())
		{
			//Mesh finded
			cmesh->meshResource = tmp->second;
		}
		else
		{
			//Need to import that mesh
			ResourceMesh* resMesh = (ResourceMesh*)app->resourceManager->createNewResource(ResourceType::RESOURCE_MESH);
			app->resourceManager->meshImporter->importMesh(mesh, resMesh);
			resMesh->originalFile.assign(file);
			meshesImported.insert(std::pair<int, ResourceMesh*>(meshIndex, resMesh));
			cmesh->meshResource = resMesh;
			resMesh->addInstance();
		}


	}

	//Recursive generate all childs GO
	for (uint j = 0; j < node->mNumChildren; ++j)
		recImport(scene, node->mChildren[j], go, basePath, file);
}

//------------------------------------

bool ImporterScene::loadResource(ResourceScene* resource)
{
	bool ret = false;

	if (!resource)
		return ret;

	std::string path(DEFAULT_PREF_SAVE_PATHS);
	path.append(resource->getExportedFile());

	_LOG(LOG_INFO, "Loading prefab/scene resource from: '%s'.", path.c_str());

	char* buffer = NULL;
	uint size = app->fs->load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		FileParser file(buffer);

		app->goManager->loadSceneOrPrefabs(file);

		ret = true;
	}

	RELEASE_ARRAY(buffer);

	return ret;
}