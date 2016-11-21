#include "Globals.h"
#include "Application.h"
#include "ModuleGOManager.h"

#include "FileParser.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

#include "JOctree.h"
#include "DrawDebug.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "OpenGL.h"

#include "Camera.h"

//TMP
#include "ModuleResourceManager.h"
#include "ImporterMesh.h"
#include "ResourceMesh.h"

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


ModuleGOManager::ModuleGOManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Manager: Creation.");
	name.assign("module_manager");

	sceneRootObject = new GameObject(NULL, 0);
	sceneRootObject->setName("SceneRootNode");
}


ModuleGOManager::~ModuleGOManager()
{
	_LOG(LOG_STD, "Manager: Destroying.");
	if (sceneRootObject)
		RELEASE(sceneRootObject);
}

bool ModuleGOManager::init(FileParser* conf)
{
	_LOG(LOG_STD, "Manager: Init.");
	//Log assimp info //TODO: remove
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

	//sceneTree = new JQuadTree();
	sceneTree = new JOctree();
	sceneTree->setRoot(AABB::FromCenterAndSize(float3(0, 40, 0), float3(100, 100, 100)));

	if (sceneRootObject)
		return true;
	else
		return false;
}

update_status ModuleGOManager::preUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::update(float dt)
{
	if (sceneRootObject)
		sceneRootObject->update(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::postUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGOManager::cleanUp()
{
	//Stop log stream
	aiDetachAllLogStreams();
	ilShutDown();

	RELEASE(sceneRootObject);

	return true;
}

void ModuleGOManager::draw()
{
	Camera* cam = (app->isPlaySate()) ? (app->renderer3D->getActiveCamera()) : (app->camera->getCamera());

	if (sceneRootObject && cam)
	{
		if (cam && cam->isCullingActive())
		{
			std::vector<GameObject*> vec;
			sceneTree->collectCandidates(vec, cam->frustum);
			for (uint i = 0; i < vec.size(); ++i)
				vec[i]->draw(false);
		}
		else
			sceneRootObject->draw(true);
	}
}

GameObject* ModuleGOManager::getSceneroot()const
{
	return sceneRootObject;
}

GameObject* ModuleGOManager::createEmptyGO()
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

GameObject* ModuleGOManager::createEmptyGoWithAABB(float xP, float yP, float zP) //TMP
{
	GameObject* ret = NULL;

	if (sceneRootObject)
	{
		if (selected)
			ret = selected->addChild();
		else
			ret = sceneRootObject->addChild();

		ret->enclosingBox.SetFromCenterAndSize(float3(xP, yP, zP), float3(2, 2, 2));
		insertGameObjectToTree(ret);
	}
	else
		_LOG(LOG_ERROR, "Can't create an empty game object because sceene root node is NULL.");

	return ret;
}

GameObject* ModuleGOManager::createCamera()
{
	GameObject* ret = NULL;

	if (sceneRootObject)
	{
		ret = sceneRootObject->addChild();
		if (ret)
		{
			ret->addComponent(CAMERA);
			ret->setName("Camera");
		}
	}
	else
		_LOG(LOG_ERROR, "Can't create an empty game object because sceene root node is NULL.");

	return ret;
}

Component* ModuleGOManager::addTransform()
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

Component* ModuleGOManager::addMesh()
{
	Mesh* ret = NULL;

	if (selected)
	{
		selected->addComponent(MESH);
	}

	return ret;
}

Component* ModuleGOManager::addMaterial()
{
	Material* ret = NULL;

	if (selected)
	{
		selected->addComponent(MATERIAL);
	}

	return ret;
}

Component* ModuleGOManager::addCamera()
{
	Material* ret = NULL;

	if (selected)
	{
		selected->addComponent(CAMERA);
	}

	return ret;
}

GameObject* ModuleGOManager::getSelected()const
{
	return selected;
}

void ModuleGOManager::select(GameObject* toSelect)
{
	selected = toSelect;
}

GameObject* ModuleGOManager::loadFBX(char* file, char* path)
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

	char* buffer;
	uint fileSize = app->fs->load(realPath, &buffer);
	const aiScene* scene = NULL;
	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		_LOG(LOG_ERROR, "Error while loading fbx.");
		return NULL;
	}
	//const aiScene* scene = aiImportFile(realPath, aiProcessPreset_TargetRealtime_MaxQuality);//TODO: fit this with own format system

	if (scene && scene->HasMeshes())
	{
		_LOG(LOG_INFO, "Loading fbx from %s.", realPath);
		root = loadObjects(scene->mRootNode, scene, sceneRootObject);

		aiReleaseImport(scene);
	}

	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(realPath);

	return root;
}

GameObject* ModuleGOManager::loadObjects(aiNode* node, const aiScene* scene, GameObject* parent)
{
	GameObject* ret = NULL;

	if (!parent)
		return ret;

	ret = parent->addChild();

	char name[256];
	sprintf_s(name, 256, "%s %d", node->mName.C_Str(), indexGO);

	ret->setName(name);

	_LOG(LOG_INFO, "Loading new game obejct: %i. ===================", indexGO);
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
		_LOG(LOG_INFO, "Loading new mesh. ------------------");
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
				m->idTexture = mat->loadTexture(clearTexPath(path));
			}

			aiColor4D col;
			scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, col);
			mat->color.Set(col.r, col.g, col.b, col.a);

			RELEASE_ARRAY(path);
		}
	}

	ret->updateAABB();
	//insertGameObjectToTree(ret); //Commented for now because object already inserted on the tree on ret->updateAABB

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		loadObjects(node->mChildren[i], scene, ret);
	}

	return ret;
}

void ModuleGOManager::drawDebug()
{
	if (sceneRootObject)
		sceneRootObject->drawDebug();

	if (showTree && sceneTree)
	{
		std::vector<AABB> boxes;
		sceneTree->collectTreeBoxes(boxes);

		for (uint i = 0; i < boxes.size(); ++i)
		{
			drawBoxDebug(boxes[i], Red);
		}

		sceneTree->coollectBoxes(boxes);

		for (uint i = 0; i < boxes.size(); ++i)
		{
			drawBoxDebug(boxes[i], Yellow);
		}
	}//DEL_COM
}

bool ModuleGOManager::deleteGameObject(GameObject* toDel)
{
	bool ret = false;

	if (!toDel)
		return ret;

	if (selected == toDel)
		select(NULL);

	GameObject* parent = toDel->getParent();

	if (parent)
	{
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

	eraseGameObjectFromTree(toDel);
	RELEASE(toDel);
	
	if (!toDel)
		ret = true;

	return ret;
}

void ModuleGOManager::makeGOShowAABoxRec(GameObject* obj, bool show)
{
	if (obj)
	{
		obj->drawEnclosingAABB = show;
		for (uint i = 0; i < obj->childrens.size(); ++i)
		{
			makeGOShowAABoxRec(obj->childrens[i], show);
		}
	}
}

void ModuleGOManager::makeGOShowOBoxRec(GameObject* obj, bool show)
{
	if (obj)
	{
		obj->drawOrientedBox = show;
		for (uint i = 0; i < obj->childrens.size(); ++i)
		{
			makeGOShowOBoxRec(obj->childrens[i], show);
		}
	}
}

void ModuleGOManager::insertGameObjectToTree(GameObject* obj)//DEL_COM
{
	if (sceneTree && obj)
		sceneTree->insert(obj);
}

void ModuleGOManager::eraseGameObjectFromTree(GameObject* obj)//DEL_COM
{
	if (sceneTree && obj)
		sceneTree->erase(obj);
}

void ModuleGOManager::saveScene(const char* name)
{
	//TODO: Supose for now that name is already usable
	if (sceneRootObject)
	{
		FileParser scene;
		//TODO: add some meta before go??
		if (sceneRootObject->saveGO(&scene))
		{
			char* buf;
			uint size = scene.writeJson(&buf, false);
			if (app->fs->save(name, buf, size) != size)
			{
				_LOG(LOG_ERROR, "Error saving the scene!");
			}
			else
			{
				_LOG(LOG_INFO, "Successfully save the scene: %s.", name);
			}
		}
		else
		{
			_LOG(LOG_ERROR, "Error saving the scene!");
		}
	}
	else
		_LOG(LOG_WARN, "No scene to save.");
}

void ModuleGOManager::loadScene(const char* name)
{

}

GameObject* ModuleGOManager::loadCube()//DEL_COM
{
	GameObject* ret = NULL;

	if (!sceneRootObject)
		return ret;

	ret = sceneRootObject->addChild();

	Mesh* mesh = (Mesh*)ret->addComponent(MESH);
	mesh->createAnEmptyMeshRes();
	mesh->meshResource->loadMeshResource("428466960.jof"); //TMP
	//mesh->meshResource->loadMeshResource("142988795.jof"); //TMP
	mesh->loadToOpenGl();

	/*const uint verticesNum = 24;
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

	float normals[verticesNum * 3]
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

	const uint uvsNum = 72;

	float uvs[uvsNum]
	{
		1.f, 1.f, 0.f, 0.f,	1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f,//Front
		//------
		0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f,//Top
		//-----
		1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,//Right
		//-----
		1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f,//Left
		//-----
		1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f,//Back
		//-----
		1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f //Bottom
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
	mat->loadTexture("Lenna.png");*/


	return ret;
}