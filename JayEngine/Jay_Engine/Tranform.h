#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "Math.h"

class Tranform : public Component
{
public:
	Tranform(GameObject* gObj);
	virtual ~Tranform();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

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

public:
	float3 position = float3::zero;
	float3 scale = float3::zero;
	Quat rotation = Quat::identity;
	float3 rotationEuler = float3::zero;
};

#endif // !__TRANSFORMATION_H__