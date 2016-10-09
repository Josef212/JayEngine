#include "GameObject.h"

#include "Tranform.h"
#include "Mesh.h"
#include "Material.h"


GameObject::GameObject(GameObject* parent) : parent(parent)
{
	name.assign("Game Object");
	init();
	addComponent(TRANSFORMATION);
	//TMP
	_LOG("GameObject created.");
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
			ret = new Tranform(this);
		}
		break;

		case MESH:
		{
			ret = new Mesh(this);
		}
		break;

		case MATERIAL:
		{
			ret = new Material(this);
		}
		break;
	}

	if (ret)
	{
		components.push_back(ret);
		ret->init();
	}

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
		return components[i];
	}

	return ret;
}

//std::vector<Component*> findComponent(ComponentType type);

/*const std::vector<Component*> GameObject::getComponents()const
{
	return components;
}*/