#include "Globals.h"
#include "Application.h"
#include "ModuleManager.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ModuleManager::ModuleManager(bool startEnabled) : Module(startEnabled)
{
	name.assign("module_manager");
}


ModuleManager::~ModuleManager()
{
}

bool ModuleManager::init()
{
	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG("Error while Devil Init: %s\n", iluErrorString(devilError));
	}

	sceneRootObject = new GameObject(NULL, app->manager->nextGOId);
	++app->manager->nextGOId;
	sceneRootObject->setName("SceneRootNode");

	if (sceneRootObject)
		return true;
	else
		return false;
}

update_status ModuleManager::preUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleManager::update(float dt)
{
	if (sceneRootObject)
		sceneRootObject->update(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleManager::postUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleManager::cleanUp()
{
	//Stop log stream
	aiDetachAllLogStreams();
	ilShutDown();

	RELEASE(sceneRootObject);

	return true;
}

GameObject* ModuleManager::getSceneroot()const
{
	return sceneRootObject;
}

GameObject* ModuleManager::createEmptyGO()
{
	GameObject* ret = NULL;
	
	if (sceneRootObject)
	{
		if (selected)
			ret = selected->addChild();
		else
			ret = sceneRootObject->addChild();
	}
	else
		_LOG("Can't create an empty game object because sceene root node is NULL.");

	return ret;
}

Component* ModuleManager::addTransform()
{
	Transform* ret = NULL;

	if (selected)
	{
		Transform* trans = (Transform*)selected->findComponent(TRANSFORMATION);
		if(trans)
			selected->addComponent(TRANSFORMATION);
	}

	return ret;
}

Component* ModuleManager::addMesh()
{
	Mesh* ret = NULL;

	if (selected)
	{
		selected->addComponent(MESH);
	}

	return ret;
}

Component* ModuleManager::addMaterial()
{
	Material* ret = NULL;

	if (selected)
	{
		selected->addComponent(MATERIAL);
	}

	return ret;
}

GameObject* ModuleManager::getSelected()const
{
	return selected;
}

void ModuleManager::select(GameObject* toSelect)
{
	selected = toSelect;
}

GameObject* ModuleManager::loadFBX(char* file, char* path)
{
	GameObject* root = NULL;

	if (!file)
	{
		_LOG("Error while loading fbx: file is NULL.");
		return root; //If path is NULL dont do nothing
	}

	char* realPath = new char[256];

	if (!path)
	{
		//Maybe in future take a default path
		strcpy_s(realPath, 256, DEFAULT_FB_PATH);
	}
	else
		strcpy_s(realPath, 256, path);

	strcat_s(realPath, 256, "/");
	strcat_s(realPath, 256, file);

	const aiScene* scene = aiImportFile(realPath, aiProcessPreset_TargetRealtime_MaxQuality);//TODO: fit this with own format system

	if (scene, scene->HasMeshes())
	{
		_LOG("Loading fbx from %s.", realPath);
		loadObjects(scene->mRootNode, scene, sceneRootObject);

		aiReleaseImport(scene);
	}

	RELEASE_ARRAY(realPath);

	return root;
}

GameObject* ModuleManager::loadObjects(aiNode* node, const aiScene* scene, GameObject* parent)
{
	GameObject* ret = NULL;

	if (!parent)
		return ret;

	ret = parent->addChild();

	ret->setName(node->mName.C_Str());

	//TODO: set transformation
	_LOG("Loading new game obejct: %i. ===================", indexGO);
	++indexGO;
	//TODO: set material
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		_LOG("Loading new mesh: %i. ------------------", indexMesh);
		Mesh* m = (Mesh*)ret->addComponent(MESH);
		m->loadMesh(scene->mMeshes[node->mMeshes[i]], true);
		//node->mMeshes is an uint array with the index of the mesh in scene->mMesh
		if (scene->HasMaterials())
		{
			Material* mat = NULL;
			mat = (Material*)ret->findComponent(MATERIAL);
			if (!mat)
				mat = (Material*)ret->addComponent(MATERIAL);
			//TODO: Clear tex path
			char* path = new char[256];
			aiString str;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			if (str.length > 0)
			{
				strcpy_s(path, 256, str.C_Str());
				m->idTexture = mat->loadTexture(path);
			}

			aiColor4D col;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			mat->color.Set(col.r, col.g, col.b, col.a);

			RELEASE_ARRAY(path);
		}
		++indexMesh;
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		loadObjects(node->mChildren[i], scene, ret);
	}

	return ret;
}