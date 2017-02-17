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

	sceneRootObject = new GameObject(nullptr, 0);
	sceneRootObject->SetName("SceneRootNode");
	currentScene.assign("scene.json");
}


ModuleGOManager::~ModuleGOManager()
{
	_LOG(LOG_STD, "Manager: Destroying.");
	if (sceneRootObject)
		RELEASE(sceneRootObject);
}

bool ModuleGOManager::Init(FileParser* conf)
{
	_LOG(LOG_STD, "Manager: Init.");

	sceneTree = new JOctree();
	sceneTree->SetRoot(AABB::FromCenterAndSize(float3(0, 0, 0), float3(1000, 1000, 1000)));

	if (sceneRootObject)
		return true;
	else
		return false;
}

bool ModuleGOManager::Start()
{
	bool ret = true;

	GameObject* cam = CreateCamera();
	if (cam)
	{
		cam->SetName("Main Camera");
		app->renderer3D->SetActiveCamera((Camera*)cam->GetComponents(CMP_CAMERA)[0]);
	}

	return ret;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	RemoveFlaggedGO();

	if (sceneRootObject && sceneRootObject->transform)
	{
		sceneRootObject->RecCalcTransform(sceneRootObject->transform->GetLocalTransform());
		sceneRootObject->RecCalcBoxes();
	}

	if (mustSaveScene)
	{
		SaveSceneNow(currentScene.c_str());
		mustSaveScene = false;
	}

	if (mustLoadScene)
	{
		LoadSceneNow(currentScene.c_str());
		mustLoadScene = false;
	}


	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
	if (app->IsPlaySate() && sceneRootObject)
		sceneRootObject->Update(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleGOManager::CleanUp()
{
	RELEASE(sceneRootObject);

	return true;
}

void ModuleGOManager::Draw()
{
	Camera* cam = (app->IsPlaySate()) ? (app->renderer3D->GetActiveCamera()) : (app->camera->GetCamera());

	if (sceneRootObject && cam)
	{
		if (cam && cam->IsCullingActive())
		{
			std::vector<GameObject*> vec;
			sceneTree->CollectCandidates(vec, cam->frustum);
			for (uint i = 0; i < vec.size(); ++i)
				vec[i]->Draw(false);
		}
		else
			sceneRootObject->Draw(true);
	}
}

void ModuleGOManager::RemoveFlaggedGO()
{
	if (sceneRootObject->removeFlag)
	{
		for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
		{
			if (sceneRootObject->childrens[i])
				sceneRootObject->childrens[i]->Remove();
		}
		sceneRootObject->removeFlag = false;
	}

	if (sceneRootObject->RecRemoveFlagged())
	{
		Event ev(Event::GAME_OBJECT_DESTROYED);
		app->SendGlobalEvent(ev);
	}
}

GameObject* ModuleGOManager::GetSceneroot()const
{
	return sceneRootObject;
}

GameObject* ModuleGOManager::CreateGameObject(GameObject* parent)
{
	if (!parent)
		parent = sceneRootObject;

	return sceneRootObject->AddChild();
}

GameObject* ModuleGOManager::CreateEmptyGO()
{
	GameObject* ret = nullptr;
	
	if (sceneRootObject)
	{
		if (selected)
			ret = selected->AddChild();
		else
			ret = sceneRootObject->AddChild();
	}
	else
		_LOG(LOG_ERROR, "Can't create an empty game object because sceene root node is nullptr.");

	return ret;
}

GameObject* ModuleGOManager::CreateCamera()
{
	GameObject* ret = nullptr;

	if (sceneRootObject)
	{
		ret = sceneRootObject->AddChild();
		if (ret)
		{
			ret->AddComponent(CMP_CAMERA);
			ret->SetName("Camera");
		}
	}
	else
		_LOG(LOG_ERROR, "Can't create an empty game object because sceene root node is nullptr.");

	return ret;
}

GameObject* ModuleGOManager::GetSelected()const
{
	return selected;
}

void ModuleGOManager::Select(GameObject* toSelect)
{
	selected = toSelect;
}

void ModuleGOManager::DrawDebug()
{
	if (sceneRootObject)
		sceneRootObject->DrawDebug();

	if (showTree && sceneTree)
	{
		std::vector<AABB> boxes;
		sceneTree->CollectTreeBoxes(boxes);

		for (uint i = 0; i < boxes.size(); ++i)
		{
			DrawBoxDebug(boxes[i], Red);
		}

		sceneTree->CoollectBoxes(boxes);

		for (uint i = 0; i < boxes.size(); ++i)
		{
			DrawBoxDebug(boxes[i], Yellow);
		}
	}//DEL_COM
}

void ModuleGOManager::MakeGOShowAABoxRec(GameObject* obj, bool show)
{
	if (obj)
	{
		obj->drawEnclosingAABB = show;
		for (uint i = 0; i < obj->childrens.size(); ++i)
		{
			MakeGOShowAABoxRec(obj->childrens[i], show);
		}
	}
}

void ModuleGOManager::MakeGOShowOBoxRec(GameObject* obj, bool show)
{
	if (obj)
	{
		obj->drawOrientedBox = show;
		for (uint i = 0; i < obj->childrens.size(); ++i)
		{
			MakeGOShowOBoxRec(obj->childrens[i], show);
		}
	}
}

void ModuleGOManager::InsertGameObjectToTree(GameObject* obj)
{
	if (sceneTree && obj)
		sceneTree->Insert(obj);
}

void ModuleGOManager::EraseGameObjectFromTree(GameObject* obj)
{
	if (sceneTree && obj)
		sceneTree->Erase(obj);
}

GameObject* ModuleGOManager::GetGameObjectFromId(UID id)
{
	return (id != 0) ? (RecFindGO(id, sceneRootObject)) : (nullptr);
}

GameObject* ModuleGOManager::RecFindGO(UID id, GameObject* go)
{
	if (go->GetGOId() == id)
		return go;

	GameObject* ret = nullptr;
	
	
	for (uint i = 0; !ret && i < go->childrens.size(); ++i)
	{
		ret = RecFindGO(id, go->childrens[i]);
	}
	

	return ret;
}

GameObject* ModuleGOManager::ValidateGO(const GameObject* point)const
{
	if (point == sceneRootObject)
		return sceneRootObject;

	for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
	{
		if (point == sceneRootObject->childrens[i])
			return (GameObject*)point;
	}

	return nullptr;
}

void ModuleGOManager::SaveScene()
{
	mustSaveScene = true;
}

void ModuleGOManager::LoadScene()
{
	mustLoadScene = true;
}

/**
	saveScene and loadScene must be used to load and save full scenes.
*/
bool ModuleGOManager::SaveSceneNow(const char* name, const char* path)
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

		//TODO: scene name?

		scene.AddArray("GameObjects");
		//TODO: add some meta before go??

		for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
		{
			if (sceneRootObject->childrens[i])
				ret = sceneRootObject->childrens[i]->SaveGO(scene);
		}

		if (ret)
		{
			char* buf = nullptr;
			uint size = scene.WriteJson(&buf, false);
			if (app->fs->Save(fullPath, buf, size) != size)
			{
				_LOG(LOG_ERROR, "Error saving the scene!");
			}
			else
			{
				_LOG(LOG_INFO, "Successfully save the scene: %s.", fullPath);
				ret = true;
			}

			RELEASE_ARRAY(buf);
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

bool ModuleGOManager::LoadSceneNow(const char* name, const char* path)
{
	bool ret = false;

	if (!name)
	{
		_LOG(LOG_ERROR, "Error loading scene, invalid name.");
		return ret;
	}

	SetCurrentScene(name); //TMP

	char fullPath[128];
	if (!path)
		sprintf_s(fullPath, "%s%s", DEFAULT_SCENE_SAVE_PATH, name);
	else
		sprintf_s(fullPath, "%s%s", path, name);

	_LOG(LOG_INFO, "Loading scene: %s.", fullPath);

	char* buffer = nullptr;
	uint size = app->fs->Load(fullPath, &buffer);

	if (size > 0 && buffer)
	{
		FileParser file(buffer);

		LoadSceneOrPrefabs(file);
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
GameObject* ModuleGOManager::LoadPrefab(const char* file, const char* path)
{
	GameObject* ret = nullptr;

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

	char* buffer = nullptr;
	uint size = app->fs->Load(fullPath, &buffer);

	if (size > 0 && buffer)
	{
		FileParser file(buffer);

		LoadSceneOrPrefabs(file);
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load the prefab: %s.", fullPath);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

void ModuleGOManager::LoadSceneOrPrefabs(const FileParser& file)
{
	//TMP: must adapt create GO 
	Select(nullptr);

	int goCount = file.GetArraySize("GameObjects");
	GameObject* tmpRoot = new GameObject(nullptr, 0);
	std::map<GameObject*, uint> relations;
	for (uint i = 0; i < goCount; ++i)
	{
		GameObject* go = tmpRoot->AddChild();
		go->LoadGO(&file.GetArray("GameObjects", i), relations);
	}

	for (std::map<GameObject*, uint>::iterator it = relations.begin(); it != relations.end(); ++it)
	{
		uint parentID = it->second;
		GameObject* go = it->first;

		if (parentID > 0)
		{
			GameObject* parentGO = RecFindGO(parentID, tmpRoot);
			if (parentGO)
				go->SetNewParent(parentGO);
		}
		else if(parentID == 0)
			go->SetNewParent(sceneRootObject);
	}

	for (uint i = 0; i < tmpRoot->childrens.size(); ++i)
	{
		if (tmpRoot->childrens[i])
			tmpRoot->childrens[i]->SetNewParent(sceneRootObject);
	}

	sceneRootObject->RecCalcTransform(sceneRootObject->transform->GetLocalTransform(), true);
	sceneRootObject->RecCalcBoxes();

	//TODO: iterate all relations go->Init(); ???
}

void ModuleGOManager::CleanRoot()
{
	if (sceneRootObject)
		sceneRootObject->Remove();
}

void ModuleGOManager::CleanRootNow()
{
	if (sceneRootObject)
	{
		for (uint i = 0; i < sceneRootObject->childrens.size(); ++i)
		{
			if (sceneRootObject->childrens[i])
				sceneRootObject->childrens[i]->Remove();
		}

		if (sceneRootObject->RecRemoveFlagged())
		{
			Event ev(Event::GAME_OBJECT_DESTROYED);
			app->SendGlobalEvent(ev);
		}
	}
}

void ModuleGOManager::OnGlobalEvent(const Event& e)
{
	RecRecieveEvent(sceneRootObject, e);

	switch (e.type)
	{
	case Event::eventType::PLAY:
		OnPlay();
		break;

	case Event::eventType::STOP:
		OnStop();
		break;

	case Event::eventType::PAUSE:
		OnPause();
		break;
	}

	selected = ValidateGO(selected);
}

void ModuleGOManager::RecRecieveEvent(GameObject* obj, const Event& e)
{
	switch (e.type)
	{
	case Event::eventType::GAME_OBJECT_DESTROYED: 
		obj->OnGameObjectDestroyed();
		break;

	default:
		break;
	}

	for (uint i = 0; i < obj->childrens.size(); ++i)
	{
		if (obj->childrens[i])
			RecRecieveEvent(obj->childrens[i], e);
	}
}

/**
	On play scene should be automatically save.
*/
void ModuleGOManager::OnPlay()
{
	SaveScene();
}

/**
	TODO: What scene should do on pause.
*/
void ModuleGOManager::OnPause()
{

}

/**
	On stop clean the scene and current scene should be automatically load to recover the editor state.
*/
void ModuleGOManager::OnStop()
{
	//First clean the scene
	CleanRootNow();

	//Second load the scene
	LoadScene();
}

/**
	TODO: Is this usefull for something. Not for now...
	For now this is just a setter for the current scene string that is stored in the module.
	This string is used to reload the scene on play-stop.
*/
bool ModuleGOManager::SetCurrentScene(const char* scene)
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

const char* ModuleGOManager::GetCurrentScene()
{
	return currentScene.c_str();
}

GameObject* ModuleGOManager::LoadCube()//TODO: Resource manager?? Save primitives there, etc.
{
	GameObject* ret = nullptr;

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