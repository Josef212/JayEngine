#include "Transform.h"

#include "FileParser.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


Transform::Transform(GameObject* gObj, int id) : Component(gObj, id)
{
	type = TRANSFORMATION;
	translation.Set(0.f, 0.f, 0.f);
	scale.Set(1.f, 1.f, 1.f);
	rotation.Set(0.f, 0.f, 0.f, 0.f);
	name.assign("Transform");
}


Transform::~Transform()
{
	if (object)
		object->transform = NULL;
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

//---------------------------------------

//Translation
float3 Transform::getLocalPosition()const
{
	return translation;
}

float3 Transform::getGlobalPosition()const
{
	return globalTransform.TranslatePart();
}

void Transform::setLocalPosition(const float3& pos)
{
	translation = pos;
	localTransformChanged = true;
}

//Scale
float3 Transform::getLocalScale()const
{
	return scale;
}

void Transform::setLocalScale(const float3& scl)
{
	scale = scl;
	localTransformChanged = true;
}

//Rotation
float3 Transform::getLocalRotation()const
{
	return editorRotation;
}

Quat Transform::getLocalQuatRotation()const
{
	return rotation;
}

void Transform::setLocalRotation(float3& eulerRot)
{
	eulerRot.x *= DEGTORAD;
	eulerRot.y *= DEGTORAD;
	eulerRot.z *= DEGTORAD;

	float3 eRot = eulerRot - editorRotation;
	Quat qRot = Quat::FromEulerXYZ(eRot.x, eRot.y, eRot.z);
	rotation = rotation * qRot;
	editorRotation = eulerRot;
	localTransformChanged = true;
}

void Transform::setLocalRotation(const Quat& rot)
{
	rotation = rot;
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformChanged = true;
}

//Transform matrix
const float4x4 Transform::getGlobalTransform()const
{
	return globalTransform;
}

const float4x4 Transform::getLocalTransform()const
{
	return localTransform;
}

void Transform::setLocalTransform(const float4x4& transform)
{
	transform.Decompose(translation, rotation, scale);
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformChanged = true;
}

//OpenGL
const float* Transform::getGlobalTransformGL()const
{
	return globalTransform.Transposed().ptr();
}


//---------------------------------------

void Transform::setTransform(aiNode* node)
{
	aiVector3D pos;
	aiVector3D scl;
	aiQuaternion rot;
	node->mTransformation.Decompose(scl, rot, pos);

	setLocalPosition(float3(pos.x, pos.y, pos.z));
	setLocalScale(float3(scl.x, scl.y, scl.z));
	setLocalRotation(Quat(rot.x, rot.y, rot.z, rot.w));

	localTransform = float4x4::FromTRS(translation, rotation, scale);
	if (object && object->getParent() && object->getParent()->transform)
	{
		updateTransform(object->getParent()->transform->getGlobalTransform());
	}
	localTransformChanged = true;
}



void Transform::updateTransform(const float4x4& parentMat)
{
	localTransformChanged = false;
	localTransform = float4x4::FromTRS(translation, rotation, scale);
	globalTransform = parentMat * localTransform;
}

bool Transform::saveCMP(FileParser& sect)
{
	bool ret = true;

	sect.addInt("comp_type", (int)type);
	sect.addBool("active", active);
	sect.addInt("UUID", id);
	sect.addInt("go_UUID", object->getGOId());

	//TODO: add float3 and quaternion
	sect.addFloat3("position", translation);
	sect.addFloat3("scale", scale);
	sect.addFloatArray("rotation", rotation.ptr(), 4);

	return ret;
}

bool Transform::loadCMP(FileParser& sect)
{
	bool ret = true;

	active = sect.getBool("active", true);
	id = sect.getInt("UUID", 0);

	translation = sect.getFloat3("position", float3::zero);
	scale = sect.getFloat3("scale", float3(1, 1, 1));
	rotation.x = sect.getFloat("rotation", 0.f, 0);
	rotation.y = sect.getFloat("rotation", 0.f, 1);
	rotation.z = sect.getFloat("rotation", 0.f, 2);
	rotation.w = sect.getFloat("rotation", 0.f, 3);

	localTransform = float4x4::FromTRS(translation, rotation, scale);

	if (object && object->getParent() && object->getParent()->transform)
	{
		updateTransform(object->getParent()->transform->getGlobalTransform());
	}

	localTransformChanged = true;

	return ret;
}