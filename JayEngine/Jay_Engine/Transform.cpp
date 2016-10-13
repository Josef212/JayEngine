#include "Transform.h"
#include "GameObject.h"


Transform::Transform(GameObject* gObj, int id) : Component(gObj, id)
{
	type = TRANSFORMATION;
	position.Set(0.f, 0.f, 0.f);
	scale.Set(1.f, 1.f, 1.f);
	rotation.Set(0.f, 0.f, 0.f, 0.f);
	name.assign("Transform");
	//TMP
	_LOG("Transform created.");
}


Transform::~Transform()
{
}

void Transform::enable()
{
	if (!active)
		active = true;
}

void Transform::disable()
{
	if (active)
		active = false;
}

void Transform::init()
{

}

void Transform::update(float dt)
{
}

void Transform::cleanUp()
{

}

void Transform::setPosition(float x, float y, float z)
{
	position.Set(x, y, z);
}

void Transform::setPosition(float* pos)
{
	position.Set(pos);
}

const void Transform::getPosition(float& x, float& y, float& z)const
{
	x = position.x;
	y = position.y;
	z = position.z;
}

float* Transform::getPosition()const
{
	return (float*)&position;
}

void Transform::setScale(float x, float y, float z)
{
	scale.Set(x, y, z);
}

void Transform::setScale(float* scl)
{
	scale.Set(scl);
}

const void Transform::getScale(float& x, float& y, float& z)const
{
	x = scale.x;
	y = scale.y;
	z = scale.z;
}

float* Transform::getScale()const
{
	return (float*)&scale;
}

void Transform::setRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);
}

void Transform::setRotation(float* rot)
{
	float3 r;
	r.Set(rot);

	while (r.x < 0)
		r.x += 360;
	while (r.y < 0)
		r.y += 360;
	while (r.z < 0)
		r.z += 360;

	while (r.x > 360)
		r.x -= 360;
	while (r.y > 360)
		r.y -= 360;
	while (r.z > 360)
		r.z -= 360;

	r.x *= DEGTORAD;
	r.y *= DEGTORAD;
	r.z *= DEGTORAD;

	rotation = Quat::FromEulerXYZ(r.x, r.y, r.z);
}

void Transform::setRotation(float x, float y, float z)
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

const void Transform::getRotation(float& x, float& y, float& z, float& w)const
{
	x = rotation.x;
	y = rotation.y;
	z = rotation.z;
	w = rotation.w;
}

float* Transform::getRotation()
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

float* Transform::getEulerRot()
{
	rotationEuler = rotation.ToEulerXYZ();

	rotationEuler.x *= RADTODEG;
	rotationEuler.y *= RADTODEG;
	rotationEuler.z *= RADTODEG;

	while (rotationEuler.x < 0)
		rotationEuler.x += 360;
	while (rotationEuler.y < 0)
		rotationEuler.y += 360;
	while (rotationEuler.z < 0)
		rotationEuler.z += 360;

	while (rotationEuler.x > 360)
		rotationEuler.x -= 360;
	while (rotationEuler.y > 360)
		rotationEuler.y -= 360;
	while (rotationEuler.z > 360)
		rotationEuler.z -= 360;

	return (float*)&rotationEuler;
}

float3 Transform::getGlobalPosition()
{
	float3 ret = position;

	if (object->getParent())
	{
		Transform* t = (Transform*)object->getParent()->findComponent(TRANSFORMATION);
		if (t)
		{
			ret += t->getGlobalPosition();
		}
	}

	return ret;
}

void Transform::getGlobalPosition(float& x, float& y, float& z)
{
	float3 pos = getGlobalPosition();

	x = pos.x;
	y = pos.y;
	z = pos.z;
}

float4x4 Transform::getTransformMatrix()const
{
	if (isEnable())
	{
		float4x4 ret = float4x4::FromTRS(position, rotation.ToFloat4x4(), scale);
		ret.Transpose();
		return ret;
	}
	else
		return float4x4::identity;
}