#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"

class Material : public Component
{
public:
	Material(GameObject* gObj, int id);
	virtual ~Material();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

private:

public:
	uint idTexture = 0;
};

#endif // !__MATERIAL_H__