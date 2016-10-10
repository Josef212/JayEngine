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
	while (rot[0] < 0)
		rot[0] += 360;
	while (rot[1] < 0)
		rot[1] += 360;
	while (rot[2] < 0)
		rot[2] += 360;

	while (rot[0] > 360)
		rot[0] -= 360;
	while (rot[1] > 360)
		rot[1] -= 360;
	while (rot[2] > 360)
		rot[2] -= 360;

	rot[0] *= DEGTORAD;
	rot[1] *= DEGTORAD;
	rot[2] *= DEGTORAD;

	rotation = Quat::FromEulerXYZ(rot[0], rot[1], rot[2]);
}

void Tranform::setRotation(float x, float y, float z)
{
	while (x < 0)
		x += 360;
	while (y < 0)
		y += 360;
	while (z < 0)
		z += 360;

	while (x > 360)
		x -= 360;
	while (y > 360)
		y -= 360;
	while (z > 360)
		z -= 360;

	x *= DEGTORAD;
	y *= DEGTORAD;
	z *= DEGTORAD;

	rotation = Quat::FromEulerXYZ(x, y, z);
}

const void Tranform::getRotation(float& x, float& y, float& z, float& w)const
{
	x = rotation.x;
	y = rotation.y;
	z = rotation.z;
	w = rotation.w;
}

float* Tranform::getRotation()
{
	float3 ret = rotation.ToEulerXYZ();

	ret.x *= RADTODEG;
	ret.y *= RADTODEG;
	ret.z *= RADTODEG;

	while (ret.x < 0)
		ret.x += 360;
	while (ret.y < 0)
		ret.y += 360;
	while (ret.z < 0)
		ret.z += 360;

	while (ret.x > 360)
		ret.x -= 360;
	while (ret.y > 360)
		ret.y -= 360;
	while (ret.z > 360)
		ret.z -= 360;

	return (float*)&ret;
}