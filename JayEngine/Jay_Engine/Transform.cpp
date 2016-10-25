#include "Transform.h"
#include "GameObject.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


Transform::Transform(GameObject* gObj, int id) : Component(gObj, id)
{
	type = TRANSFORMATION;
	position.Set(0.f, 0.f, 0.f);
	scale.Set(1.f, 1.f, 1.f);
	rotation.Set(0.f, 0.f, 0.f, 0.f);
	name.assign("Transform");
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

void Transform::setTransform(aiNode* node)
{
	aiVector3D pos;
	aiVector3D scl;
	aiQuaternion rot;
	node->mTransformation.Decompose(scl, rot, pos);

	setPosition(pos.x, pos.y, pos.z);
	setScale(scl.x, scl.y, scl.z);
	setRotation(rot.x, rot.y, rot.z, rot.w);

	localTransform = float4x4::FromTRS(position, rotation, scale);

	float3 dadPos;
	float3 dadScale;
	Quat dadRot;

	for (aiNode* tmp = node->mParent; tmp; tmp = tmp->mParent)
	{
		tmp->mTransformation.Decompose(scl, rot, pos);
		dadPos.Set(pos.x, pos.y, pos.z);
		dadScale.Set(scl.x, scl.y, scl.z);
		dadRot.Set(rot.x, rot.y, rot.z, rot.w);

		float4x4 dadMat = float4x4::FromTRS(dadPos, dadRot, dadScale);
		parentTransform = dadMat * parentTransform;
	}

	worldTransform = parentTransform * localTransform;
	worldTransform = worldTransform.Transposed();

}

void Transform::setPosition(float x, float y, float z)
{
	position.Set(x, y, z);
	updateTransform(parentTransform);
}

void Transform::setPosition(float* pos)
{
	position.Set(pos);
	updateTransform(parentTransform);
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
	updateTransform(parentTransform);
}

void Transform::setScale(float* scl)
{
	scale.Set(scl);
	updateTransform(parentTransform);
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
	updateTransform(parentTransform);
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

	while (r.x >= 360)
		r.x -= 360;
	while (r.y >= 360)
		r.y -= 360;
	while (r.z >= 360)
		r.z -= 360;

	r.x *= DEGTORAD;
	r.y *= DEGTORAD;
	r.z *= DEGTORAD;

	rotation = Quat::FromEulerXYZ(r.x, r.y, r.z);
	updateTransform(parentTransform);
}

void Transform::setRotation(float x, float y, float z)
{
	while (x < 0)
		x += 360;
	while (y < 0)
		y += 360;
	while (z < 0)
		z += 360;

	while (x >= 360)
		x -= 360;
	while (y >= 360)
		y -= 360;
	while (z >= 360)
		z -= 360;

	x *= DEGTORAD;
	y *= DEGTORAD;
	z *= DEGTORAD;

	rotation = Quat::FromEulerXYZ(x, y, z);
	updateTransform(parentTransform);
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

	while (ret.x >= 360)
		ret.x -= 360;
	while (ret.y >= 360)
		ret.y -= 360;
	while (ret.z >= 360)
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

	while (rotationEuler.x >= 360)
		rotationEuler.x -= 360;
	while (rotationEuler.y >= 360)
		rotationEuler.y -= 360;
	while (rotationEuler.z >= 360)
		rotationEuler.z -= 360;

	return (float*)&rotationEuler;
}

float4x4 Transform::getTransformMatrix()
{
	if (isEnable())
	{
		return worldTransform;
	}
	else
		return float4x4::identity;
}

void Transform::updateTransform(float4x4& parentMat)
{
	parentTransform = parentMat;
	localTransform = float4x4::FromTRS(position, rotation, scale);
	worldTransform = parentTransform * localTransform;

	for (uint i = 0; i < object->childrens.size(); ++i)
	{
		Transform* trans = object->childrens[i]->getTransform();
		if (trans)
		{
			trans->updateTransform(worldTransform);
		}
	}
	worldTransform = worldTransform.Transposed();
}