#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "Math.h"

class FileParser;

class Transform : public Component
{
public:
	Transform(GameObject* gObj, int id);
	virtual ~Transform();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

	//----------------------------------------

	//Translation
	float3 getLocalPosition()const;
	float3 getGlobalPosition()const;

	void setLocalPosition(const float3& pos);

	//Scale
	float3 getLocalScale()const;

	void setLocalScale(const float3& scl);

	//Rotation
	float3 getLocalRotation()const;
	Quat getLocalQuatRotation()const;

	void setLocalRotation(float3& eulerRot);
	void setLocalRotation(const Quat& rot);

	//Transform matrix
	const float4x4 getGlobalTransform()const;
	const float4x4 getLocalTransform()const;

	void setLocalTransform(const float4x4& transform);

	//OpenGL
	const float* getGlobalTransformGL()const;

	//----------------------------------------


	void updateTransform(const float4x4& parentMat);

	bool saveCMP(FileParser& sect);
	bool loadCMP(FileParser& sect);

private:

public:

	bool localTransformChanged = true;

private:
	float3 translation = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
	float3 editorRotation = float3::zero;

	float4x4 globalTransform = float4x4::identity;
	float4x4 localTransform = float4x4::identity;

};

#endif // !__TRANSFORMATION_H__