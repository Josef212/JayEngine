#include "Globals.h"
#include "Application.h"
#include "ModuleManager.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


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

	sceneRootObject = new GameObject(NULL);
	sceneRootObject->setName("RootNode");

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
		ret = new GameObject(sceneRootObject);
	else
		_LOG("Can't create an empty game object because sceene root node is NULL.");

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

GameObject* ModuleManager::loadFBX(const char* file, const char* path)
{
	GameObject* root = NULL;



	return root;
}