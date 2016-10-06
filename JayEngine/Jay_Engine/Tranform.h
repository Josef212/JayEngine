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
	const void getPosition(float& x, float& y, float& z)const;

	void setScale(float x, float y, float z);
	const void getScale(float& x, float& y, float& z)const;

	void setRotation(float x, float y, float z, float w);
	const void getRotation(float& x, float& y, float& z, float& w)const;

public:
	/*float3 position;
	float3 scale;
	Quat rotation;*/
};

#endif // !__TRANSFORMATION_H__