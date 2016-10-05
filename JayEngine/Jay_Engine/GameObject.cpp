#include "GameObject.h"

#include "Tranform.h"
#include "Mesh.h"
#include "Material.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::init()
{

}

void GameObject::update(float dt)
{

}

void GameObject::cleanUp()
{

}

Component* GameObject::addComponent(ComponentType type)
{
	Component* ret = NULL;

	switch (type)
	{
		case UNKNOWN:
			LOG("Error while creating component, no component type.");
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

	return ret;
}

bool GameObject::removeComponent(Component* comp)
{
	bool ret = true;

	if (comp)
	{
		uint pos = 0;
		for (; pos < components.size(); ++pos)
		{
			if (components[pos] == comp)
			{
				break;
			}
		}
		components.erase(components.begin() + pos);
	}
	else
		ret = false;

	return ret;
}

std::vector<Component*> GameObject::findComponent(ComponentType type)
{
	std::vector<Component*> ret;

	for (uint i = 0; i < components.size(); ++i)
	{
		ret.push_back(components[i]);
	}

	return ret;
}