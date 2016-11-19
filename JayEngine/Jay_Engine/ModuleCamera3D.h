#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Math.h"

class GameObject;
class Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool init(FileParser* conf);
	bool start();
	update_status update(float dt);
	bool cleanUp();

	void look(const float3& position, const float3& reference, bool rotateAroundReference = false);
	void lookAt(const float3& spot);
	void lookAt(float dx, float dy);
	void move(float dt);
	void rotate(float dt);
	void setPos(const float3& pos);
	void orbit(float dx, float dy);
	void zoom(float dt);

	GameObject* getEditorCameraObj()const;
	Camera* getCamera();

private:

public:
	float moveSpeed = 10.f;
	float rotSpeed = 3.f; //TODO: read this from config
	float zoomSpeed = 500.f;
	float distToReference = 10.f;

private:
	GameObject* defaultCamera = NULL;
	Camera* defaultCameraComp = NULL;

};

#endif // !__MODULECAMERA3D_H__