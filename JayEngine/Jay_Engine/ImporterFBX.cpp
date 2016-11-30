#include "Application.h"
#include "ImporterFBX.h"

#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"

#include "ImporterMesh.h"
#include "ImporterTexture.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

//TODO: get cameras from the fbx??

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

bool ImporterFBX::importFBX(const char* fullPath, const char* fileName) //TODO: path full ??
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


GameObject* ImporterFBX::importFBXRec(aiNode* node, const aiScene* scene, GameObject* parent, const char* originalFBX)
{
	GameObject* ret = NULL;

	if (!parent)
		return ret;

	ret = parent->addChild();

	ret->setName(node->mName.C_Str());

	//Setting transform
	Transform* trans = ret->getTransform();
	if (!trans)
		trans = (Transform*)ret->findComponent(TRANSFORMATION)[0];
	if (!trans)
		trans = (Transform*)ret->addComponent(TRANSFORMATION);

	if (trans)
		trans->setTransform(node);

	//Lets set meshes and materials. This time will convert them. Resource info will be in each component when save
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		//For each mesh will import it and create a mesh component
		aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* cpMesh = (Mesh*)ret->addComponent(MESH);
		ResourceMesh* resMesh = cpMesh->createAnEmptyMeshRes();

		app->resourceManager->meshImporter->importMesh(aMesh, resMesh); //TODO: set origin file to resource
		resMesh->originalFile.assign(originalFBX);

		//All this should first check if the meshes or the textures are already imported
		if (scene->HasMaterials())
		{
			//Create whe component as well import the texture
			Material* cpMat = (Material*)ret->addComponent(MATERIAL);
			ResourceTexture* resTex = cpMat->createAnEmptyMaterialRes(); //All this should first check if the meshes or the textures are already imported

			aiColor4D col;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			cpMat->color.Set(col.r, col.g, col.b, col.a);

			aiString str;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			char texPath[128];
			strcpy_s(texPath, 128, str.C_Str());
			app->resourceManager->textureImporter->importTexture(clearTexPath(texPath), resTex); //TODO: check if already in memory

			//TODO: Mesh should have an index of the texture??

		}
	}

	ret->updateAABB(); //So that we can save it later

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		importFBXRec(node->mChildren[i], scene, ret, originalFBX);
	}

	return ret;
}