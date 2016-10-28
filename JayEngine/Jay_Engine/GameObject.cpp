#include "Application.h"
#include "GameObject.h"

#include "DrawDebug.h"
#include "ModuleRenderer3D.h"

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ModuleManager.h"


GameObject::GameObject(GameObject* parent, int id) : parent(parent), id(id)
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
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->update(dt);
	}
	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->update(dt);
	}

	if (transform && transform->transformUpdated)
		updateAABB();

}

void GameObject::cleanUp()
{
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->cleanUp();
	}
	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->cleanUp();
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
			ret = new Transform(this, nextCompId);
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

	ret = new GameObject(this, app->manager->nextGOId);
	childrens.push_back(ret);
	++app->manager->nextGOId;

	return ret;
}

bool GameObject::removeComponent(Component* comp)
{
	bool ret = false;

	if (comp)
	{
		uint pos = 0;
		for (; pos < components.size(); ++pos)
		{
			if (components[pos] == comp)
			{
				ret = true;
				break;
			}
		}
		if (ret)
		{
			components[pos]->cleanUp();
			components.erase(components.begin() + pos);
		}
	}
	else
		ret = false;

	return ret;
}

/*Component* GameObject::findComponent(ComponentType type)
{
	Component* ret = NULL;

	for (uint i = 0; i < components.size(); ++i)
	{
		if(components[i]->type == type)
			return components[i];
	}

	return ret;
}*/

std::vector<Component*> GameObject::findComponent(ComponentType type)
{
	std::vector<Component*> ret;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
			ret.push_back(components[i]);
	}

	if (ret.empty())
		ret.push_back(NULL);

	return ret;
}

/*const std::vector<Component*> GameObject::getComponents()const
{
	return components;
}*/

GameObject* GameObject::getParent() const
{
	return parent;
}

void GameObject::draw()
{
	if (!isGOActive())
		return;

	app->renderer3D->drawGameObject(this);
	
	for (uint i = 0; i < childrens.size(); ++i)
		if (childrens[i])
			childrens[i]->draw();
}

void GameObject::drawDebug()
{
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->drawDebug();
	}

	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->debugDraw();
	}

	if (drawEnclosingAABB)
		drawBoxDebug(enclosingBox);
	if (drawOrientedBox)
		drawBoxDebug(orientedBox);
}

void GameObject::updateAABB() //TODO: make enclose for all meshes
{
	enclosingBox.SetNegativeInfinity();

	Mesh* m = (Mesh*)findComponent(MESH)[0];
	if (m)
		enclosingBox.Enclose((float3*)m->vertices, m->numVertices);

	orientedBox = enclosingBox;
	orientedBox.Transform(transform->getTransformMatrix().Transposed());
	enclosingBox.SetFrom(orientedBox);

	for (std::vector<GameObject*>::iterator it = childrens.begin(); it != childrens.end(); ++it)
		(*it)->updateAABB();

	transform->transformUpdated = false;
}