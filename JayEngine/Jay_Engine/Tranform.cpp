#include "Tranform.h"



Tranform::Tranform(GameObject* gObj) : Component(gObj)
{
	type = TRANSFORMATION;
}


Tranform::~Tranform()
{
}

void Tranform::enable()
{
	if (!active)
		active = true;
}

void Tranform::disable()
{
	if (active)
		active = false;
}

void Tranform::init()
{

}

void Tranform::update(float dt)
{

}

void Tranform::cleanUp()
{

}

void Tranform::setPosition(float x, float y, float z)
{
	position.Set(x, y, z);
}

const void Tranform::getPosition(float& x, float& y, float& z)const
{
	x = position.x;
	y = position.y;
	z = position.z;
}

void Tranform::setScale(float x, float y, float z)
{
	scale.Set(x, y, z);
}

const void Tranform::getScale(float& x, float& y, float& z)const
{
	x = scale.x;
	y = scale.y;
	z = scale.z;
}

void Tranform::setRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
}

const void Tranform::getRotation(float& x, float& y, float& z, float& w)const
{
	x = rotation.x;
	y = rotation.y;
	z = rotation.z;
	w = rotation.w;
}