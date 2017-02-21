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

	// Name analysis to handle FBX dummy nodes
	// check bottom of http://g3d.cs.williams.edu/g3d/G3D10/assimp.lib/code/FBXImportSettings.h
	static const char* dummies[5] = {
		"$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; ++i)
	{
		if (name.find(dummies[i]) != std::string::npos && node->mNumChildren == 1)
		{
			node = node->mChildren[0];

			node->mTransformation.Decompose(scaling, rotation, translation);

			pos += float3(translation.x, translation.y, translation.z);
			scl += float3(scaling.x, scaling.y, scaling.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			name = node->mName.C_Str();
			i = -1;
		}
	}

	GameObject* go = parent->AddChild();
	go->SetName(name.c_str());

	Transform* trans = go->transform;

	trans->SetLocalPosition(pos);
	trans->SetLocalScale(scl);
	trans->SetLocalRotation(rot);

	//TODO: Load meta data

	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		//int meshIndex = node->mMeshes[i];
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

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
		{
			childGO = go;
		}

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

			if (str.data[0] == '*')
			{
				uint n = atoi(&str.data[1]);
				if (n < scene->mNumTextures)
				{
					aiTexture* tex = scene->mTextures[n];
					UID id = app->resourceManager->ImportBuffer((const char*)tex->pcData,
						(tex->mHeight == 0) ? tex->mWidth : tex->mHeight * tex->mWidth,
						RESOURCE_TEXTURE);

					cMat->SetResource(id);
				}
			}
			else
			{
				std::string file(basePath);
				file += str.C_Str();
				if (!app->fs->Exist(file.c_str()))
				{
					std::string extracted;
					app->fs->SplitPath(str.C_Str(), nullptr, &extracted);
					file = basePath;
					file += extracted;
				}

				cMat->SetResource(app->resourceManager->ImportFile(file.c_str()));
				Resource* tmp = (Resource*)cMat->GetResource();
				if(tmp)tmp->RemoveInstance();
			}
		}


		//Add mesh
		Mesh* cmesh = (Mesh*)childGO->AddComponent(ComponentType::CMP_MESH);
		cmesh->SetResource(app->resourceManager->ImportBuffer(mesh, 0, RESOURCE_MESH, (basePath + file).c_str()));
		Resource* tmp = (Resource*)cmesh->GetResource();
		tmp->RemoveInstance();
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