#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class GameObject;
class Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool init();
	bool start();
	update_status update(float dt);
	bool cleanUp();

	void look(const vec3 &position, const vec3 &reference, bool rotateAroundReference = false);
	void lookAt(const vec3 &spot);
	void move(const vec3 &movement);
	void setPos(const vec3 &pos);
	float* getViewMatrix();

private:

	void calculateViewMatrix();

public:
	
	float maxDistanceToVehicle = 25.0f;
	vec3 X, Y, Z, position, reference;

	//TMP
	GameObject* defaultCamera = NULL;
	Camera* cameraComp = NULL;

private:
	mat4x4 viewMatrix, viewMatrixInverse;

};

#endif // !__MODULECAMERA3D_H__