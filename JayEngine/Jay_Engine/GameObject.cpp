#include "Application.h"
#include "GameObject.h"

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"


#include "ModuleManager.h"


GameObject::GameObject(GameObject* parent, int id) : parent(parent), id(id)
{
	name.assign("Game Object");
	init();
	addComponent(TRANSFORMATION);
}


GameObject::~GameObject()
{
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
		name.assign("str");
}

int GameObject::getGOId()const
{
	return id;
}

Component* GameObject::addComponent(ComponentType type)
{
	Component* ret = NULL;

	switch (type)
	{
		case UNKNOWN:
			_LOG("Error while creating component, no component type.");
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
	}

	if (ret)
	{
		++nextCompId;
		components.push_back(ret);
		ret->init();
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

Component* GameObject::findComponent(ComponentType type)
{
	Component* ret = NULL;

	for (uint i = 0; i < components.size(); ++i)
	{
		if(components[i]->type == type)
			return components[i];
	}

	return ret;
}

//std::vector<Component*> findComponent(ComponentType type);

/*const std::vector<Component*> GameObject::getComponents()const
{
	return components;
}*/

GameObject* GameObject::getParent() const
{
	return parent;
}