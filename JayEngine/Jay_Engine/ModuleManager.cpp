#include "Globals.h"
#include "Application.h"
#include "ModuleManager.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "OpenGL.h"

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
	_LOG(LOG_STD, "Manager: Creation.");
	name.assign("module_manager");
}


ModuleManager::~ModuleManager()
{
	_LOG(LOG_STD, "Manager: Destroying.");
	if (sceneRootObject)
		RELEASE(sceneRootObject);
}

bool ModuleManager::init()
{
	_LOG(LOG_STD, "Manager: Init.");
	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG(LOG_ERROR, "Error while Devil Init: %s\n", iluErrorString(devilError));
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
		_LOG(LOG_ERROR, "Can't create an empty game object because sceene root node is NULL.");

	return ret;
}

Component* ModuleManager::addTransform()
{
	Transform* ret = NULL;

	if (selected)
	{
		Transform* trans = selected->getTransform();
		if(!trans)
			trans = (Transform*)selected->findComponent(TRANSFORMATION)[0];

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
		_LOG(LOG_ERROR, "Error while loading fbx: file is NULL.");
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
		_LOG(LOG_MANAGER, "Loading fbx from %s.", realPath);
		root = loadObjects(scene->mRootNode, scene, sceneRootObject);

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

	char name[256];
	sprintf_s(name, 256, "%s %d", node->mName.C_Str(), indexGO);

	ret->setName(name);

	_LOG(LOG_MANAGER, "Loading new game obejct: %i. ===================", indexGO);
	++indexGO;

	//Set transformation
	Transform* trans = ret->getTransform();
	if(!trans)
		trans = (Transform*)ret->findComponent(TRANSFORMATION)[0];

	if (trans)
		trans->setTransform(node);

	//Set material
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		_LOG(LOG_MANAGER, "Loading new mesh. ------------------");
		Mesh* m = (Mesh*)ret->addComponent(MESH);
		m->loadMesh(scene->mMeshes[node->mMeshes[i]], true);
		//node->mMeshes is an uint array with the index of the mesh in scene->mMesh
		if (scene->HasMaterials())
		{
			Material* mat = (Material*)ret->addComponent(MATERIAL);
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
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		loadObjects(node->mChildren[i], scene, ret);
	}

	return ret;
}

bool ModuleManager::deleteGameObject(GameObject* toDel)
{
	bool ret = false;

	if (!toDel)
		return ret;

	if (selected == toDel)
		select(NULL);

	if (toDel->getParent())
	{
		GameObject* parent = toDel->getParent();

		uint i = 0;
		for (; i < parent->childrens.size(); ++i)
		{
			if (parent->childrens[i] == toDel)
			{
				parent->childrens.erase(parent->childrens.begin() + i);
				break;
			}
		}		
	}

	RELEASE(toDel);
	
	if (!toDel)
		ret = true;

	return ret;
}

GameObject* ModuleManager::loadCube()
{
	GameObject* ret = NULL;

	if (!sceneRootObject)
		return ret;

	ret = sceneRootObject->addChild();

	Mesh* mesh = (Mesh*)ret->addComponent(MESH);

	const uint verticesNum = 24;
	const uint indicesNum = 36;
	float s = 0.5;
	float vertex[verticesNum] = {
		s, s, s,	//0
		s, -s, s,	//1
		-s, -s, s,	//2
		-s, s, s,	//3
		s, s, -s,	//4
		-s, s, -s,	//5
		s, -s, -s,	//6
		-s, -s, -s	//7
	};

	uint index[indicesNum]{
		0, 2, 1, 0, 3, 2,	//Front
		3, 0, 4, 3, 4, 5,	//Top
		4, 0, 1, 4, 1, 6,	//Right
		3, 5, 7, 3, 7, 2,	//Left
		5, 4, 6, 5, 6, 7,	//Back
		1, 2, 7, 1, 7, 6	//Bottom
	};

	float normals[verticesNum]
	{
		1, 1, 1,
		1, -1, 1,
		-1, -1, 1,
		-1, 1, 1, 
		1, 1, -1, 
		-1, 1, -1,
		1, -1, -1,
		-1, -1, -1
	};

	const uint uvsNum = 48;

	float uvs[uvsNum]
	{
			0, 0,
			1, 0,
			0, 1, 
			1, 1, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0, 
			1, 1, 
			0, 1, 
			0, 0, 
			1, 0

	};

	mesh->numVertices = verticesNum;
	mesh->vertices = new float[mesh->numVertices];

	glGenBuffers(1, (GLuint*)&mesh->idVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesNum, vertex, GL_STATIC_DRAW);

	mesh->numIndices = indicesNum;
	mesh->indices = new uint[mesh->numIndices];

	glGenBuffers(1, (GLuint*)&mesh->idIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indicesNum, index, GL_STATIC_DRAW);

	mesh->numNormals = verticesNum;
	mesh->normals = new float[mesh->numNormals];

	glGenBuffers(1, (GLuint*)&mesh->idNormals);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numNormals, normals, GL_STATIC_DRAW);

	mesh->numTexCoords = uvsNum;
	mesh->texCoords = new float[mesh->numTexCoords];

	glGenBuffers(1, (GLuint*)&mesh->idTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->numTexCoords, uvs, GL_STATIC_DRAW);

	Material* mat = (Material*)ret->addComponent(MATERIAL);
	mat->loadTexture("Lenna.png");

	return ret;
}