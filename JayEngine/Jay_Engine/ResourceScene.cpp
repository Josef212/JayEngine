#include "Application.h"
#include "ResourceScene.h"

#include "ImporterScene.h"
#include "ModuleResourceManager.h"

ResourceScene::ResourceScene(UID uuid) : Resource(uuid, RESOURCE_SCENE)
{
}


ResourceScene::~ResourceScene()
{
}

void ResourceScene::Save(FileParser& file)
{
	Resource::Save(file);
}

void ResourceScene::Load(FileParser& file)
{
	Resource::Load(file);
}

bool ResourceScene::LoadInMemory()
{
	return app->resourceManager->sceneImporter->LoadResource(this);
}