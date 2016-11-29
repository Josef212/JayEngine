#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "Math.h"

struct aiNode;
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

	void setTransform(aiNode* node);

	//All these transform are about local transform
	void setPosition(float x, float y, float z);
	void setPosition(float* pos);
	const void getPosition(float& x, float& y, float& z)const;
	float* getPosition()const;

	void setScale(float x, float y, float z);
	void setScale(float* scl);
	const void getScale(float& x, float& y, float& z)const;
	float* getScale()const;

	void setRotation(float x, float y, float z, float w);
	void setRotation(float x, float y, float z);
	void setRotation(float* rot);
	const void getRotation(float& x, float& y, float& z, float& w)const;
	float* getRotation();//Don't use it
	float* getEulerRot();

	float4x4 getTransformMatrix();
	float4x4 getLocalMatrix();
	void updateTransform(float4x4& parentMat);

	bool saveCMP(FileParser& sect);
	bool loadCMP(FileParser& sect);

private:

public:
	float3 position = float3::zero;
	float3 scale = float3::zero;
	Quat rotation = Quat::identity;
	float3 rotationEuler = float3::zero;

	bool transformUpdated = false;

private:
	float4x4 worldTransform = float4x4::identity;
	float4x4 localTransform = float4x4::identity;
	float4x4 parentTransform = float4x4::identity;
};

#endif // !__TRANSFORMATION_H__