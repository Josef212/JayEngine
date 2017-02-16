#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "Component.h"
#include "Math.h"

class FileParser;

class Transform : public Component
{
public:
	Transform(GameObject* gObj, int id);
	virtual ~Transform();

	void Enable()override;

	void Init()override;
	void Update(float dt)override;
	void CleanUp()override;

	//----------------------------------------

	//Translation
	float3 GetLocalPosition()const;
	float3 GetGlobalPosition()const;

	void SetLocalPosition(const float3& pos);

	//Scale
	float3 GetLocalScale()const;

	void SetLocalScale(const float3& scl);

	//Rotation
	float3 GetLocalRotation()const;
	Quat GetLocalQuatRotation()const;

	void SetLocalRotation(float3& eulerRot);
	void SetLocalRotation(const Quat& rot);

	//Transform matrix
	const float4x4 GetGlobalTransform()const;
	const float4x4 GetLocalTransform()const;

	void SetLocalTransform(const float4x4& transform);

	//OpenGL
	const float* GetGlobalTransformGL()const;

	//----------------------------------------


	void UpdateTransform(const float4x4& parentMat);

	bool SaveCMP(FileParser& sect)override;
	bool LoadCMP(FileParser& sect)override;

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

#endif // !__TRANSFORM__