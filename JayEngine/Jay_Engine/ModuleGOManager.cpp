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


ModuleGOManager::ModuleGOManager(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Manager: Creation.");
	name.assign("module_manager");

	sceneRootObject = new GameObject(NULL, 0);
	sceneRootObject->setName("SceneRootNode");
	currentScene.assign("scene.json");
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

	//sceneTree = new JQuadTree();
	sceneTree = new JOctree();
	sceneTree->setRoot(AABB::FromCenterAndSize(float3(0, 40, 0), float3(100, 100, 100)));

	if (sceneRootObject)
		return true;
	else
		return false;
}

bool ModuleGOManager::start()
{
	bool ret = true;

	GameObject* cam = createCamera();
	if (cam)
	{
		cam->setName("Main Camera");
		app->renderer3D->setActiveCamera((Camera*)cam->findComponent(CAMERA)[0]);
	}

	return ret;
}

update_status ModuleGOManager::preUpdate(float dt)
{	
	if (sceneRootObject && sceneRootObject->transform)
	{
		sceneRootObject->recCalcTransform(sceneRootObject->transform->getLocalTransform());
		sceneRootObject->recCalcBoxes();
	}

	removeFlaggedGO();

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::update(float dt)
{
	if (app->isPlaySate() && sceneRootObject)
		sceneRootObject->update(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::postUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGOManager::cleanUp()
{
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

void ModuleGOManager::removeFlaggedGO()
{
	if (sceneRootObject->removeFlag)
	{
		for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
		{
			if (sceneRootObject->childrens[i])
				sceneRootObject->childrens[i]->remove();
		}
		sceneRootObject->removeFlag = false;
	}

	if (sceneRootObject->recRemoveFlagged())
	{
		Event ev(Event::GAME_OBJECT_DESTROYED);
		app->sendGlobalEvent(ev);
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
		Transform* trans = selected->transform;
		if(!trans)
			trans = (Transform*)selected->findComponent(TRANSFORMATION)[0];

		if(!trans)
			ret = (Transform*)selected->addComponent(TRANSFORMATION);
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

GameObject* ModuleGOManager::getGameObjectFromId(UID id)
{
	return (id != 0) ? (recFindGO(id, sceneRootObject)) : (NULL);
}

GameObject* ModuleGOManager::recFindGO(UID id, GameObject* go)
{
	if (go->getGOId() == id)
		return go;

	GameObject* ret = NULL;
	
	
	for (uint i = 0; !ret && i < go->childrens.size(); ++i)
	{
		ret = recFindGO(id, go->childrens[i]);
	}
	

	return ret;
}

GameObject* ModuleGOManager::validateGO(const GameObject* point)const
{
	if (point == sceneRootObject)
		return sceneRootObject;

	for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
	{
		if (point == sceneRootObject->childrens[i])
			return (GameObject*)point;
	}

	return NULL;
}


/**
	saveScene and loadScene must be used to load and save full scenes.
*/
bool ModuleGOManager::saveScene(const char* name, const char* path)
{
	bool ret = false;

	if (!name)
	{
		_LOG(LOG_ERROR, "Error saving scene, invalid name.");
		return ret;
	}

	if (sceneRootObject)
	{
		char fullPath[128];
		if (!path)
			sprintf_s(fullPath, "%s%s", DEFAULT_SCENE_SAVE_PATH, name);
		else
			sprintf_s(fullPath, "%s%s", path, name);

		_LOG(LOG_INFO, "Saving scene: %s.", fullPath);

		FileParser scene;
		scene.addArray("GameObjects");
		//TODO: add some meta before go??
		if (sceneRootObject->saveGO(scene))
		{
			char* buf;
			uint size = scene.writeJson(&buf, false);
			if (app->fs->save(fullPath, buf, size) != size)
			{
				_LOG(LOG_ERROR, "Error saving the scene!");
			}
			else
			{
				_LOG(LOG_INFO, "Successfully save the scene: %s.", fullPath);
				ret = true;
			}
		}
		else
		{
			_LOG(LOG_ERROR, "Error saving the scene!");
		}
	}
	else
		_LOG(LOG_WARN, "No scene to save.");

	return ret;
}

bool ModuleGOManager::loadScene(const char* name, const char* path)
{
	bool ret = false;

	if (!name)
	{
		_LOG(LOG_ERROR, "Error loading scene, invalid name.");
		return ret;
	}

	setCurrentScene(name); //TMP

	char fullPath[128];
	if (!path)
		sprintf_s(fullPath, "%s%s", DEFAULT_SCENE_SAVE_PATH, name);
	else
		sprintf_s(fullPath, "%s%s", path, name);

	_LOG(LOG_INFO, "Loading scene: %s.", fullPath);

	char* buffer = NULL;
	uint size = app->fs->load(fullPath, &buffer);

	if (size > 0 && buffer)
	{
		FileParser file(buffer);

		loadSceneOrPrefabs(file);
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load the prefab: %s.", fullPath);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}


/**
	loadPrefabs should be used to load objects like fbx that have been exported.
*/
GameObject* ModuleGOManager::loadPrefab(const char* file, const char* path)
{
	GameObject* ret = NULL;

	//-------------------------------------------------

	if (!file)
	{
		_LOG(LOG_ERROR, "Invalid file name.");
		return ret;
	}

	char fullPath[128];
	if (!path)
		sprintf_s(fullPath, "%s", DEFAULT_PREF_SAVE_PATHS);
	else
		sprintf_s(fullPath, "%s", path);

	strcat_s(fullPath, file);
	
	_LOG(LOG_INFO, "Loading a prefab: %s.", fullPath);

	//-------------------------------------------------

	char* buffer = NULL;
	uint size = app->fs->load(fullPath, &buffer);

	if (size > 0 && buffer)
	{
		FileParser file(buffer);

		loadSceneOrPrefabs(file);
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load the prefab: %s.", fullPath);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

void ModuleGOManager::loadSceneOrPrefabs(FileParser& file)
{
	std::vector<GameObject*> tmpGO;

	int goCount = file.getArraySize("GameObjects");
	GameObject* rootTMP = new GameObject(NULL, 0);
	for (uint i = 0; i < goCount; ++i)
	{
		GameObject* gO = rootTMP->addChild();
		gO->loadGO(file.getArray("GameObjects", i));
		tmpGO.push_back(gO);
	}

	for (uint i = 0; i < goCount; ++i)
	{
		FileParser p = file.getArray("GameObjects", i);
		UID pID = p.getInt("parent_UUID", 0);
		if (pID != 0)
		{
			GameObject* tmp = recFindGO(pID, rootTMP);
			if (!tmp)
				tmp = getGameObjectFromId(pID);

			if (tmp)
			{
				tmpGO[i]->setNewParent(tmp);
			}
		}
		else
			tmpGO[i]->setNewParent(sceneRootObject);
	}

	if (sceneRootObject && sceneRootObject->transform)
	{
		sceneRootObject->recCalcTransform(sceneRootObject->transform->getLocalTransform(), true);
		sceneRootObject->recCalcBoxes();
	}

	RELEASE(rootTMP);
}

void ModuleGOManager::cleanRoot() //TODO: Should destroy all scene directly or wait until next update.
{
	//NOTE-REALLY IMPORTANT: Check all pointer references of game objects in all the code
	//Probably should be in game objects and components deletes

	select(NULL);
	app->renderer3D->setActiveCamera(NULL);

	for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
	{
		if (sceneRootObject->childrens[i])
		{
			if (sceneRootObject->childrens[i] == selected)
				select(NULL);

			RELEASE(sceneRootObject->childrens[i]);
		}
	}
	sceneRootObject->childrens.clear();
}

void ModuleGOManager::onGlobalEvent(const Event& e)
{
	recRecieveEvent(sceneRootObject, e);

	switch (e.type)
	{
	case Event::eventType::PLAY:
		onPlay();
		break;

	case Event::eventType::STOP:
		onStop();
		break;

	case Event::eventType::PAUSE:
		onPause();
		break;
	}

	selected = validateGO(selected);
}

void ModuleGOManager::recRecieveEvent(GameObject* obj, const Event& e)
{
	switch (e.type)
	{
	case Event::eventType::GAME_OBJECT_DESTROYED: 
		obj->onGameObjectDestroyed();
		break;

	default:
		break;
	}

	for (uint i = 0; i < obj->childrens.size(); ++i)
	{
		if (obj->childrens[i])
			recRecieveEvent(obj->childrens[i], e);
	}
}

/**
	On play scene should be automatically save.
*/
void ModuleGOManager::onPlay()
{
	saveScene(currentScene.c_str());
}

/**
	TODO: What scene should do on pause.
*/
void ModuleGOManager::onPause()
{

}

/**
	On stop clean the scene and current scene should be automatically load to recover the editor state.
*/
void ModuleGOManager::onStop()
{
	//First clean the scene
	cleanRoot();

	//Second load the scene
	loadScene(currentScene.c_str());
}

/**
	TODO: Is this usefull for something. Not for now...
	For now this is just a setter for the current scene string that is stored in the module.
	This string is used to reload the scene on play-stop.
*/
bool ModuleGOManager::setCurrentScene(const char* scene)
{
	bool ret = false;

	if (!scene)
	{
		_LOG(LOG_WARN, "Invalid scene name to load.");
		ret = false;
	}
	else
	{
		if (currentScene == scene)
		{
			_LOG(LOG_WARN, "This scene is the current scene: %s.", scene); //TODO: Maybe if we want to load the current scene again we should reload it.
		}
		else
		{
			currentScene.assign(scene); //TODO: Load the new scene???
			_LOG(LOG_INFO, "Current scene has changed: %s.", scene);
			ret = true;
		}
	}

	return ret;
}

const char* ModuleGOManager::getCurrentScene()
{
	return currentScene.c_str();
}

GameObject* ModuleGOManager::loadCube()//TODO: Resource manager?? Save primitives there, etc.
{
	GameObject* ret = NULL;

	if (!sceneRootObject)
		return ret;

	//ret = sceneRootObject->addChild();

	//Mesh* mesh = (Mesh*)ret->addComponent(MESH);
	//mesh->createAnEmptyMeshRes();
	//mesh->meshResource->loadMeshResource("428466960.jof"); //TMP
	//mesh->meshResource->loadMeshResource("142988795.jof"); //TMP
	//mesh->loadToOpenGl();

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