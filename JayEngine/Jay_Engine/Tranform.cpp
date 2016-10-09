#include "Tranform.h"



Tranform::Tranform(GameObject* gObj) : Component(gObj)
{
	type = TRANSFORMATION;
	position.Set(0.f, 0.f, 0.f);
	scale.Set(1.f, 1.f, 1.f);
	rotation.Set(0.f, 0.f, 0.f, 0.f);
	//TMP
	_LOG("Transform created.");
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

void Tranform::setPosition(float* pos)
{
	position.Set(pos);
}

const void Tranform::getPosition(float& x, float& y, float& z)const
{
	x = position.x;
	y = position.y;
	z = position.z;
}

float* Tranform::getPosition()const
{
	return (float*)&position;
}

void Tranform::setScale(float x, float y, float z)
{
	scale.Set(x, y, z);
}

void Tranform::setScale(float* scl)
{
	scale.Set(scl);
}

const void Tranform::getScale(float& x, float& y, float& z)const
{
	x = scale.x;
	y = scale.y;
	z = scale.z;
}

float* Tranform::getScale()const
{
	return (float*)&scale;
}

void Tranform::setRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
}

void Tranform::setRotation(float* rot)
{
	//Will change that
}

const void Tranform::getRotation(float& x, float& y, float& z, float& w)const
{
	x = rotation.x;
	y = rotation.y;
	z = rotation.z;
	w = rotation.w;
}

float* Tranform::getRotation()const
{
	return (float*)&rotation;
}