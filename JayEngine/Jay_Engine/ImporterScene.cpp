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
#include "ResourceMesh.h"
#include "ResourceTexture.h"

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
		sprintf_s(name, 128, "%d.%s", resUID, SCENE_EXTENSION);
		exportedFile.assign(name);
		char fullPath[256];
		sprintf_s(fullPath, 256, "%s%s", DEFAULT_PREF_SAVE_PATHS, name);

		char* buf = NULL;
		uint _size = save.writeJson(&buf, false); //TODO: When finish testing change to fast mode.

		if (app->fs->save(fullPath, buf, size) == size)
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
				if (app->fs->exist(texPath.c_str()))
				{
					//If exist import it. Might check if i should change the path before.
					std::string texFile;
					app->fs->splitPath(texPath.c_str(), NULL, &texFile);
					
					cMat->textureResource = (ResourceTexture*)app->resourceManager->getResourceFromUID(app->resourceManager->importFile(texFile.c_str(), true));
				}
				else
				{
					_LOG(LOG_ERROR, "Texture '%s' does not exist.", texPath.c_str());
				}
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
		}


	}

	//Recursive generate all childs GO
	for (uint j = 0; j < node->mNumChildren; ++j)
		recImport(scene, node->mChildren[j], go, basePath, file);
}

bool ImporterScene::importFBX(const char* fullPath, const char* fileName) //TODO: path full ??
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
		file.addArray("GameObjects");
		_LOG(LOG_STD, "Loading fbx: %s.", fullPath);

		GameObject* root = new GameObject(NULL, app->resourceManager->getNewUID()); //TODO: Should clean this because it adds a new GO

		importFBXRec(scene->mRootNode, scene, root, fullPath);

		meshesImported.clear();

		//Saving the file
		root->saveGO(file);

		char* buf;
		uint jSize = file.writeJson(&buf, false);

		char jFileName[128];
		strcpy_s(jFileName, 128, DEFAULT_PREF_SAVE_PATHS);
		strcat_s(jFileName, 128, fileName);

		if (app->fs->save(jFileName, buf, jSize) != jSize)
		{
			_LOG(LOG_ERROR, "Error saving json file: %s.", );
		}

		aiReleaseImport(scene);
		RELEASE_ARRAY(buffer);
		RELEASE_ARRAY(buf);
	}
	else
	{
		_LOG(LOG_ERROR, "Error loading fbx: %s.", fullPath);
		ret = false;
	}

	return ret;
}


GameObject* ImporterScene::importFBXRec(aiNode* node, const aiScene* scene, GameObject* parent, const char* originalFBX)
{
	GameObject* ret = NULL;

	if (!parent)
		return ret;

	ret = parent->addChild();

	ret->setName(node->mName.C_Str());

	//Setting transform
	Transform* trans = ret->transform;
	if (!trans)
		trans = (Transform*)ret->findComponent(TRANSFORMATION)[0];
	if (!trans)
		trans = (Transform*)ret->addComponent(TRANSFORMATION);

	if (trans)
	{
		aiVector3D pos;
		aiVector3D scl;
		aiQuaternion rot;
		node->mTransformation.Decompose(scl, rot, pos);

		trans->setLocalPosition(float3(pos.x, pos.y, pos.z));
		trans->setLocalScale(float3(scl.x, scl.y, scl.z));
		trans->setLocalRotation(Quat(rot.x, rot.y, rot.z, rot.w));
	}

	//Lets set meshes and materials. This time will convert them. Resource info will be in each component when save
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		//For each mesh will import it and create a mesh component
		Mesh* cpMesh = (Mesh*)ret->addComponent(MESH);

		int index = node->mMeshes[i];
		std::map<int, ResourceMesh*>::iterator tmp = meshesImported.find(index);
		if (tmp != meshesImported.end())			//If mesh has already been imported, take that resource, else import it
		{
			cpMesh->meshResource = tmp->second;
		}
		else
		{
			aiMesh* aMesh = scene->mMeshes[index];
			ResourceMesh* resMesh = cpMesh->createAnEmptyMeshRes();

			app->resourceManager->meshImporter->importMesh(aMesh, resMesh);
			resMesh->originalFile.assign(originalFBX);
			meshesImported.insert(std::pair<int, ResourceMesh*>(index, resMesh));
		}

		//All this should first check if the meshes or the textures are already imported
		if (scene->HasMaterials())
		{
			//Create whe component as well import the texture
			Material* cpMat = (Material*)ret->addComponent(MATERIAL);

			aiColor4D col;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			cpMat->color.Set(col.r, col.g, col.b, col.a);

			aiString str;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			if (str.length > 0)
			{
				std::map<std::string, ResourceTexture*>::iterator tmp2 = texturesImported.find(str.C_Str());
				if (tmp2 != texturesImported.end())
				{
					cpMat->textureResource = tmp2->second;
				}
				else
				{
					ResourceTexture* resTex = cpMat->createAnEmptyMaterialRes();
					char texPath[128];
					strcpy_s(texPath, 128, str.C_Str());
					app->resourceManager->textureImporter->importTexture(clearTexPath(texPath), resTex); //TODO: check if already in memory
					texturesImported.insert(std::pair<std::string, ResourceTexture*>(str.C_Str(), resTex)); //Dont clear the map after importing the fbx because if another fbx use the same texture will not importe it twcie
				}
			}
			//TODO: Mesh should have an index of the texture??

		}
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		importFBXRec(node->mChildren[i], scene, ret, originalFBX);
	}

	return ret;
}



