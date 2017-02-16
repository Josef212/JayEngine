#ifndef __LIGHT__
#define __LIGHT__

#include "Color.h"
#include "Math.h"

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	float3 position = float3::zero;

	int ref;
	bool on;
};

#endif // !__LIGHT__