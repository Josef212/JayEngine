#include "Application.h"
#include "GameObject.h"

#include "DrawDebug.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"
#include "RandGen.h"

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ResourceMesh.h"

#include "ModuleGOManager.h"

//TMP
#include "FileParser.h"


GameObject::GameObject(GameObject* parent, uint32 id) : parent(parent), id(id)
{
	name.assign("Game Object");
	init();
	transform = (Transform*)addComponent(TRANSFORMATION);
}


GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); ++i)
		RELEASE(components[i]);

	for (uint j = 0; j < childrens.size(); ++j)
		RELEASE(childrens[j]);
}

void GameObject::init()
{
}

void GameObject::update(float dt)
{
	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->update(dt);
	}
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->update(dt);
	}
}

void GameObject::cleanUp()
{
	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->cleanUp();
	}

	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->cleanUp();
	}
}

const char* GameObject::getName()const
{
	return name.c_str();
}

void GameObject::setName(const char* str)
{
	if (str)
		name.assign(str);
}

int GameObject::getGOId()const
{
	return id;
}

bool GameObject::isGOActive()
{
	return goActive;
}

void GameObject::setGOEnable(bool set)
{
	goActive = set;
}

Component* GameObject::addComponent(ComponentType type)
{
	Component* ret = NULL;

	switch (type)
	{
		case UNKNOWN:
			_LOG(LOG_ERROR, "Error while creating component, no component type.");
		break;

		case TRANSFORMATION:
		{
			if (hasComponent(TRANSFORMATION) == 0)
			{
				ret = new Transform(this, nextCompId);
				transform = (Transform*)ret;
			}
			else
				_LOG(LOG_WARN, "Can't add transform component, already has one.");
		}
		break;

		case MESH:
		{
			ret = new Mesh(this, nextCompId);
		}
		break;

		case MATERIAL:
		{
			ret = new Material(this, nextCompId);
		}
		break;

		case CAMERA:
		{
			ret = new Camera(this, nextCompId);
		}
		break;
	}

	if (ret)
	{
		++nextCompId;
		components.push_back(ret);
	}

	return ret;
}

GameObject* GameObject::addChild()
{
	GameObject* ret = NULL;

	ret = new GameObject(this, app->random->getRandInt());
	childrens.push_back(ret);

	return ret;
}

void GameObject::remove()
{
	removeFlag = true;
}

bool GameObject::recRemoveFlagged()
{
	bool ret = false;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->removeFlag)
		{
			components[i]->cleanUp();
			RELEASE(components[i]);
			components.erase(components.begin() + i);
		}
	}

	for (uint i = 0; i < childrens.size(); ++i)
	{
		GameObject* tmp = childrens[i];
		if (tmp && tmp->removeFlag)
		{
			tmp->cleanUp();
			app->goManager->eraseGameObjectFromTree(tmp);
			RELEASE(tmp);
			childrens.erase(childrens.begin() + i);
			ret = true;
		}
		else
		{
			ret |= tmp->recRemoveFlagged();
		}
	}

	return ret;
}

void GameObject::onGameObjectDestroyed()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->onGameObjectDestroyed();
}

std::vector<Component*> GameObject::findComponent(ComponentType type)
{
	std::vector<Component*> ret;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->type == type)
			ret.push_back(components[i]);
	}

	if (ret.empty())
		ret.push_back(NULL);

	return ret;
}

int GameObject::hasComponent(ComponentType type)
{
	int ret = 0;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
			++ret;
	}

	return ret;
}

GameObject* GameObject::getParent() const
{
	return parent;
}

void GameObject::draw(bool drawChilds) //Only use this if culling is not active
{
	if (!isGOActive())
		return;

	app->renderer3D->drawGameObject(this);
		
	if (drawChilds)
	{
		for (uint i = 0; i < childrens.size(); ++i)
			if (childrens[i])
				childrens[i]->draw(true);
	}
}

void GameObject::drawDebug()
{
	for (uint j = 0; j < components.size(); ++j)
	{
		if (components[j])
			components[j]->debugDraw();
	}

	for (uint i = 0; i < childrens.size(); ++i)
	{
		if(childrens[i])
			childrens[i]->drawDebug();
	}

	if (drawEnclosingAABB)
		drawBoxDebug(enclosingBox, Green);
	if (drawOrientedBox)
		drawBoxDebug(orientedBox, ClearBlue);
}

void GameObject::recCalcTransform(const float4x4& parentTrans, bool force)
{
	if (transform && transform->localTransformChanged || force)
	{
		force = true;
		goWasDirty = true;
		transform->updateTransform(parentTrans);

		for (uint i = 0; i < components.size(); ++i)
			if (components[i])
				components[i]->onTransformUpdate(transform);

		app->goManager->eraseGameObjectFromTree(this);
		app->goManager->insertGameObjectToTree(this);
	}
	else
		goWasDirty = false;

	for (uint i = 0; i < childrens.size(); ++i)
		if (childrens[i] && transform)
			childrens[i]->recCalcTransform(transform->getGlobalTransform(), force);
}

void GameObject::recCalcBoxes()
{
	if (goWasDirty)
	{
		recalcBox();

		orientedBox = enclosingBox;
		if (orientedBox.IsFinite() && transform)
		{
			orientedBox.Transform(transform->getGlobalTransform());
			enclosingBox.SetFrom(orientedBox);
		}
	}

	for (uint i = 0; i < childrens.size(); ++i)
		if (childrens[i] && transform)
			childrens[i]->recCalcBoxes();
}

void GameObject::recalcBox()
{
	enclosingBox.SetNegativeInfinity();

	//Iterate all components and in each mesh add into AABB enclosing box all meshes aabb
	for (uint i = 0; i < components.size(); ++i)
	{
		Component* cmp = components[i];
		if (cmp && cmp->isEnable())
			cmp->getBox(enclosingBox);
	}
}

void GameObject::setNewParent(GameObject* newParent, bool force)
{
	if (newParent == parent)
		return;

	if (parent)
	{
		std::vector<GameObject*>::iterator it = std::find(parent->childrens.begin(), parent->childrens.end(), this);
		if (it != parent->childrens.end())
			parent->childrens.erase(it);
	}

	parent = newParent;

	if (newParent)
		newParent->childrens.push_back(this);

	goWasDirty = true;

	if (force && transform && newParent && newParent->transform)
	{
		float4x4 tmp = transform->getGlobalTransform();
		transform->setLocalTransform(tmp * newParent->transform->getLocalTransform().Inverted());
	}
}

bool GameObject::saveGO(FileParser& file, std::map<uint, uint>* duplicate)const
{
	bool ret = true;

	FileParser f;

	uint uidToSave = id;
	uint parentID = (parent) ? parent->getGOId() : 0;

	if (duplicate)
	{
		uidToSave = app->random->getRandInt();
		(*duplicate)[id] = uidToSave;

		std::map<uint, uint>::iterator it = duplicate->find(parentID);
		if (it != duplicate->end())
			parentID = it->second;
	}

	f.addInt("UUID", id);
	f.addInt("parent_id", parentID);

	f.addString("name", name.c_str());

	f.addArray("components");

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i])
		{
			FileParser cmp;
			components[i]->saveCMP(cmp);
			f.addArrayEntry(cmp);
		}
	}

	file.addArrayEntry(f);

	for (uint i = 0; i < childrens.size(); ++i)
	{
		if (childrens[i])
			childrens[i]->saveGO(file, duplicate);
	}

	return ret;
}

bool GameObject::loadGO(FileParser* file, std::map<GameObject*, uint>& relations)
{
	bool ret = true;

	id = file->getInt("UUID", id);
	uint parent = file->getInt("parent_id", 0);
	relations[this] = parent;

	name = file->getString("name", "unnamed");

	int cmpCount = file->getArraySize("components");
	for (uint i = 0; i < cmpCount; ++i)
	{
		FileParser cmp(file->getArray("components", i));
		ComponentType type = (ComponentType)cmp.getInt("comp_type", -1);
		if (type != UNKNOWN)
		{
			if (type == TRANSFORMATION)
				transform->loadCMP(cmp);
			else
			{
				Component* c = addComponent(type);
				c->loadCMP(cmp);
			}
		}
		else
			_LOG(LOG_ERROR, ("Unknown component type for game object %s.!!", name.c_str()));
	}

	return ret;
}