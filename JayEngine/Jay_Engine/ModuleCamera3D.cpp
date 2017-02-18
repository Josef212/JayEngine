#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGOManager.h"
#include "ModuleEditor.h"

#include "FileParser.h"

#include "GameObject.h"
#include "Camera.h"

ModuleCamera3D::ModuleCamera3D(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Camera3D: Creation.");

	name.assign("module_camera3d");

	if (!defaultCamera)
		defaultCamera = new GameObject(nullptr, 0); 
	//NOTE: editor camera is not in the scene tree so must be deleted manually.
	//This camera should never be the camera of the game!!!

	if (defaultCamera)
	{
		defaultCamera->SetName("Editor camera");
		defaultCameraComp = (Camera*)defaultCamera->AddComponent(CMP_CAMERA);
		if(defaultCameraComp)
			defaultCameraComp->Look(float3::zero, float3(0, 10, 10));
	}
}

ModuleCamera3D::~ModuleCamera3D()
{
	_LOG(LOG_STD, "Camera3D: Destroying.");
}

bool ModuleCamera3D::Init(FileParser* conf)
{
	moveSpeed = conf->GetFloat("mov_speed", 10.f);
	rotSpeed = conf->GetFloat("rot_speed", 3.f);
	zoomSpeed = conf->GetFloat("zoom_speed", 500.f);
	distToReference = conf->GetFloat("dist_ref", 10.f);

	if (!defaultCamera || !defaultCameraComp)
		return false;
	else
		return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	_LOG(LOG_STD, "Camera3D: Start.");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	_LOG(LOG_STD, "Camera3D: CleanUp.");

	defaultCamera->CleanUp();
	RELEASE(defaultCamera);

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if(!app->editor->UsingKeyboard())
		Move(dt);
	
	if (!app->editor->UsingMouse())
	{
		Rotate(dt);
		Zoom(dt);
	}

	//TODO: picking

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3& position, const float3& reference, bool rotateAroundReference)
{
	defaultCameraComp->Look(position, reference);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3& spot)
{
	defaultCameraComp->LookAt(spot);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(float dt)
{
	Frustum* frust = &defaultCameraComp->frustum;

	float speed;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = moveSpeed / 2;
	else
		speed = moveSpeed;

	float3 movement(float3::zero);
	float3 forw(frust->front);
	float3 right(frust->WorldRight());

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forw;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forw;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (!movement.Equals(float3::zero))
	{
		movement *= (speed * dt);
		frust->Translate(movement);
	}
}

void ModuleCamera3D::Rotate(float dt)
{
	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int motX = app->input->GetMouseXMotion();
		int motY = app->input->GetMouseYMotion();

		if (!(motX != 0 && motY != 0))
			return;

		float speed;
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = rotSpeed / 2;
		else
			speed = rotSpeed;

		float x = (float)-motX * speed * dt;
		float y = (float)-motY * speed * dt;

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			Orbit(x, y);
		else
			LookAt(x, y);
	}
}

void ModuleCamera3D::SetPos(const float3& pos)
{
	
}

void ModuleCamera3D::Orbit(float dx, float dy)
{
	//TODO: improve this: better orbit rotation and rotate with an object reference
	float3 reference = defaultCameraComp->frustum.pos + defaultCameraComp->frustum.front * distToReference;
	Quat roty(defaultCameraComp->frustum.up, dx);
	Quat rotx(defaultCameraComp->frustum.WorldRight(), dx);
	float3 vec = defaultCameraComp->frustum.pos - reference;

	vec = rotx.Transform(vec);
	vec = roty.Transform(vec);

	defaultCameraComp->frustum.pos = vec + reference;
	LookAt(reference);
}

void ModuleCamera3D::LookAt(float dx, float dy)
{
	//dx will be rotation along x axis
	if (dx != 0.f)
	{
		Quat rot = Quat::RotateY(dx);
		defaultCameraComp->frustum.front = rot.Mul(defaultCameraComp->frustum.front).Normalized();
		defaultCameraComp->frustum.up = rot.Mul(defaultCameraComp->frustum.up).Normalized();
	}

	//dy will be rotation along y axis
	//more complex as the frustum up changes
	if (dy != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(defaultCameraComp->frustum.WorldRight(), dy);

		defaultCameraComp->frustum.front = rot.Mul(defaultCameraComp->frustum.front).Normalized();
		defaultCameraComp->frustum.up = rot.Mul(defaultCameraComp->frustum.up).Normalized();
	}
}

void ModuleCamera3D::Zoom(float dt)
{
	if (app->input->GetWheelYMotion() != 0)
	{
		Frustum* frust = &defaultCameraComp->frustum;

		float speed;
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = zoomSpeed / 2;
		else
			speed = zoomSpeed;

		float3 movement(float3::zero);
		float3 forw(frust->front);

		movement += (forw * app->input->GetWheelYMotion());

		if (!movement.Equals(float3::zero))
		{
			movement *= (speed * dt);
			frust->Translate(movement);
		}
	}
}

//=================================================================
GameObject* ModuleCamera3D::GetEditorCameraObj()const
{
	return defaultCamera;
}

Camera* ModuleCamera3D::GetCamera()
{
	return defaultCameraComp;
}