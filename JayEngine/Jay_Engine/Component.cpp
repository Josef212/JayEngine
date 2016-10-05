#include "Component.h"



Component::Component(GameObject* gObj) : object(gObj)
{
}


Component::~Component()
{
}

const bool Component::isEnable()const
{
	return active;
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

void Component::init()
{

}

void Component::update(float dt)
{

}

void Component::cleanUp()
{

}