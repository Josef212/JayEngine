#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool start();
	update_status update(float dt);
	bool cleanUp();

	void look(const vec3 &_position, const vec3 &_reference, bool rotateAroundReference = false);
	void lookAt(const vec3 &_spot);
	void move(const vec3 &_movement);
	void setPos(const vec3 &_pos);
	float* getViewMatrix();

private:

	void calculateViewMatrix();

public:
	
	float maxDistanceToVehicle = 25.0f;
	vec3 X, Y, Z, position, reference;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};

#endif // !__MODULECAMERA3D_H__