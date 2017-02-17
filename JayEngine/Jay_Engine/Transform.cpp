#include "Transform.h"

#include "FileParser.h"
#include "GameObject.h"

Transform::Transform(GameObject* gObj, int id) : Component(gObj, id)
{
	type = CMP_TRANSFORMATION;
	translation.Set(0.f, 0.f, 0.f);
	scale.Set(1.f, 1.f, 1.f);
	rotation.Set(0.f, 0.f, 0.f, 1.f);
	name.assign("Transform");
}


Transform::~Transform()
{
	if (object)
		object->transform = NULL;
}

void Transform::Enable()
{
	if (!selfActive)
		selfActive = true;

	localTransformChanged = true;	
}

void Transform::Init()
{

}

void Transform::Update(float dt)
{
}

void Transform::CleanUp()
{

}

//---------------------------------------

//Translation
float3 Transform::GetLocalPosition()const
{
	return translation;
}

float3 Transform::GetGlobalPosition()const
{
	return globalTransform.TranslatePart();
}

void Transform::SetLocalPosition(const float3& pos)
{
	translation = pos;
	localTransformChanged = true;
}

//Scale
float3 Transform::GetLocalScale()const
{
	return scale;
}

void Transform::SetLocalScale(const float3& scl)
{
	scale = scl;
	localTransformChanged = true;
}

//Rotation
float3 Transform::GetLocalRotation()const
{
	return editorRotation;
}

Quat Transform::GetLocalQuatRotation()const
{
	return rotation;
}

void Transform::SetLocalRotation(float3& eulerRot)
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

void Transform::SetLocalRotation(const Quat& rot)
{
	rotation = rot;
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformChanged = true;
}

//Transform matrix
const float4x4 Transform::GetGlobalTransform()const
{
	return globalTransform;
}

const float4x4 Transform::GetLocalTransform()const
{
	return localTransform;
}

void Transform::SetLocalTransform(const float4x4& transform)
{
	transform.Decompose(translation, rotation, scale);
	editorRotation = rotation.ToEulerXYZ().Abs();
	localTransformChanged = true;
}

//OpenGL
const float* Transform::GetGlobalTransformGL()const
{
	return globalTransform.Transposed().ptr();
}


//---------------------------------------

void Transform::UpdateTransform(const float4x4& parentMat)
{
	localTransformChanged = false;
	localTransform = float4x4::FromTRS(translation, rotation, scale);
	globalTransform = parentMat * localTransform;
}

bool Transform::SaveCMP(FileParser& sect)
{
	bool ret = true;

	sect.AddInt("comp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("UUID", id);
	sect.AddInt("go_UUID", object->GetGOId());

	sect.AddFloat3("position", translation);
	sect.AddFloat3("scale", scale);
	sect.AddFloatArray("rotation", rotation.ptr(), 4);

	return ret;
}

bool Transform::LoadCMP(FileParser& sect)
{
	bool ret = true;

	selfActive = sect.GetBool("active", true);
	id = sect.GetInt("UUID", 0);

	SetLocalPosition(sect.GetFloat3("position", float3::zero));
	SetLocalScale(sect.GetFloat3("scale", float3(1, 1, 1)));
	Quat r;
	r.x = sect.GetFloat("rotation", 0.f, 0);
	r.y = sect.GetFloat("rotation", 0.f, 1);
	r.z = sect.GetFloat("rotation", 0.f, 2);
	r.w = sect.GetFloat("rotation", 1.f, 3);

	SetLocalRotation(r);

	return ret;
}