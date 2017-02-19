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


ImporterScene::ImporterScene() : Importer()
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

bool ImporterScene::Import(const char* originalFile, std::string& exportedFile, const char* originalFileExtension, UID& resUID)
{
	bool ret = false;

	if (!originalFile || !originalFileExtension)
		return ret;

	char* buffer = nullptr;
	uint size = app->fs->Load(originalFile, &buffer);

	const aiScene* scene = nullptr;

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
		app->fs->SplitPath(originalFile, &path, &file);

		meshesImported.clear();

		GameObject* go = app->goManager->CreateGameObject();
		go->SetName(file.c_str());

		//Recursive create game objects.
		RecImport(scene, scene->mRootNode, go, path, file);

		//TODO: Process bones.

		//TODO: Process animations.

		aiReleaseImport(scene);

		//Serialize Game Objects recursively.
		FileParser save;
		save.AddArray("GameObjects");

		for (uint i = 0; i < go->childs.size(); ++i)
			if (go->childs[i])
				go->childs[i]->SaveGO(save);

		resUID = app->resourceManager->GetNewUID();

		char name[128];
		sprintf_s(name, 128, "%d%s", resUID, EXTENSION_SCENE);
		exportedFile.assign(name);
		char fullPath[256];
		sprintf_s(fullPath, 256, "%s%s", PATH_LIBRARY_PREFABS, name);

		char* buf = nullptr;
		uint _size = save.WriteJson(&buf, false); //TODO: When finish testing change to fast mode.

		if (app->fs->Save(fullPath, buf, _size) == _size)
			ret = true;

		RELEASE_ARRAY(buf);

		go->Remove();
	}

	return ret;
}

void ImporterScene::RecImport(const aiScene* scene, const aiNode* node, GameObject* parent, const std::string& basePath, const std::string& file)
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

	GameObject* go = parent->AddChild();
	go->SetName(name.c_str());

	Transform* trans = go->transform;

	trans->SetLocalPosition(pos);
	trans->SetLocalScale(scl);
	trans->SetLocalRotation(rot);

	//TODO: Load meta data

	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		int meshIndex = node->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[meshIndex];

		GameObject* childGO = nullptr;

		if (node->mNumMeshes > 1)
		{
			name = mesh->mName.C_Str();
			if (name.length() == 0)
			{
				name = node->mName.C_Str();
				name += "_submesh";
			}
			childGO = go->AddChild();
			childGO->SetName(name.c_str());
		}
		else
			childGO = go;

		//TODO: Adapt this to material resource.

		if (scene->HasMaterials())
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE); //TODO: Someday add other textures as normal maps etc

			Material* cMat = (Material*)childGO->AddComponent(ComponentType::CMP_MATERIAL);

			aiColor4D col;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			cMat->color.Set(col.r, col.g, col.b, col.a);

			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			if (str.length > 0)
			{
				//Need to import that texture
				std::string texPath(str.C_Str());
				app->fs->NormalizePath(texPath);

				//If exist import it. Might check if i should change the path before.
				std::string texFile;
				app->fs->SplitPath(texPath.c_str(), nullptr, &texFile);
				
				cMat->textureResource = (ResourceTexture*)app->resourceManager->GetResourceFromUID(app->resourceManager->ImportFile(texFile.c_str(), true));
				cMat->textureResource->AddInstance();
			}

			//TODO:Check assimp for embedded textures... For now will normally import textures.
			//Add texture
		}


		//Add mesh
		Mesh* cmesh = (Mesh*)childGO->AddComponent(ComponentType::CMP_MESH);
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
			ResourceMesh* resMesh = (ResourceMesh*)app->resourceManager->CreateNewResource(ResourceType::RESOURCE_MESH);
			app->resourceManager->meshImporter->ImportMesh(mesh, resMesh);
			resMesh->originalFile.assign(file);
			meshesImported.insert(std::pair<int, ResourceMesh*>(meshIndex, resMesh));
			cmesh->meshResource = resMesh;
			resMesh->AddInstance();
		}


	}

	//Recursive generate all childs GO
	for (uint j = 0; j < node->mNumChildren; ++j)
		RecImport(scene, node->mChildren[j], go, basePath, file);
}

//------------------------------------

bool ImporterScene::LoadResource(Resource* resource)
{
	bool ret = false;

	if (!resource)
		return ret;

	std::string path(PATH_LIBRARY_PREFABS);
	path.append(resource->exportedFile.c_str());

	_LOG(LOG_INFO, "Loading prefab/scene resource from: '%s'.", path.c_str());

	char* buffer = nullptr;
	uint size = app->fs->Load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		FileParser file(buffer);

		app->goManager->LoadSceneOrPrefabs(file);

		ret = true;
	}

	RELEASE_ARRAY(buffer);

	return ret;
}