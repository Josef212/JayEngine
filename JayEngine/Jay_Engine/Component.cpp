#include "Component.h"



Component::Component(GameObject* gObj, int id) : object(gObj), id(id)
{
}


Component::~Component()
{
}

const bool Component::isEnable()const
{
	return active;
}

const char* Component::getName()
{
	return name.c_str();
}

void Component::setName(const char* str)
{
	if (str)
	{
		name.assign(str);
	}
}

int Component::getId()
{
	return id;
}

void Component::enable()
{
	if (!active)
		active = true;
}

void Component::disable()
{
	if (active)
		active = false;
}

void Component::switchActive()
{
	if (active)
		disable();
	else
		enable();
}

void Component::init()
{

}

void Component::update(float dt)
{

}

void Component::cleanUp()
{

}