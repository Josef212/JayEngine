#ifndef __MODULECAMERA3D__
#define __MODULECAMERA3D__

#include "Module.h"
#include "Globals.h"
#include "Math.h"

class GameObject;
class Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Init(FileParser* conf)override;
	bool Start()override;
	update_status Update(float dt)override;
	bool CleanUp()override;

	void Look(const float3& position, const float3& reference, bool rotateAroundReference = false);
	void LookAt(const float3& spot);
	void LookAt(float dx, float dy);
	void Move(float dt);
	void Rotate(float dt);
	void SetPos(const float3& pos);
	void Orbit(float dx, float dy);
	void Zoom(float dt);

	GameObject* GetEditorCameraObj()const;
	Camera* GetCamera();

private:

public:
	float moveSpeed = 10.f;
	float rotSpeed = 3.f;
	float zoomSpeed = 500.f;
	float distToReference = 10.f;

private:
	GameObject* defaultCamera = nullptr;
	Camera* defaultCameraComp = nullptr;

};

#endif // !__MODULECAMERA3D__