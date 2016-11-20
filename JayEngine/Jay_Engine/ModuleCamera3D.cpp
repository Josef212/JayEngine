#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGOManager.h"

#include "GameObject.h"
#include "Camera.h"

ModuleCamera3D::ModuleCamera3D(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Camera3D: Creation.");

	name.assign("module_camera3d");

	if (!defaultCamera)
		defaultCamera = new GameObject(NULL, 0); 
	//NOTE: editor camera is not in the scene tree so must be deleted manually.
	//This camera should never be the camera of the game!!!

	if (defaultCamera)
	{
		defaultCamera->setName("Editor camera");
		defaultCameraComp = (Camera*)defaultCamera->addComponent(CAMERA);
		if(defaultCameraComp)
			defaultCameraComp->look(float3::zero, float3(0, 10, 10));
	}
}

ModuleCamera3D::~ModuleCamera3D()
{
	_LOG(LOG_STD, "Camera3D: Destroying.");
}

bool ModuleCamera3D::init(FileParser* conf)
{
	if (!defaultCamera || !defaultCameraComp)
		return false;
	else
		return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::start()
{
	_LOG(LOG_STD, "Camera3D: Start.");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::cleanUp()
{
	_LOG(LOG_STD, "Camera3D: CleanUp.");

	defaultCamera->cleanUp();
	RELEASE(defaultCamera);

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::update(float dt)
{
	//TODO: check if imgui is using the keyboard
	move(dt);
	//TODO: check if imgui is using the mouse
	rotate(dt);
	//TODO: mouse wheel
	zoom(dt);
	//TODO: picking

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::look(const float3& position, const float3& reference, bool rotateAroundReference)
{
	defaultCameraComp->look(position, reference);
}

// -----------------------------------------------------------------
void ModuleCamera3D::lookAt( const float3& spot)
{
	defaultCameraComp->lookAt(spot);
}


// -----------------------------------------------------------------
void ModuleCamera3D::move(float dt)
{
	Frustum* frust = &defaultCameraComp->frustum;

	float speed;
	if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = moveSpeed / 2;
	else
		speed = moveSpeed;

	float3 movement(float3::zero);
	float3 forw(frust->front);
	float3 right(frust->WorldRight());

	if (app->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forw;
	if (app->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forw;
	if (app->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (app->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (app->input->getKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (app->input->getKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (!movement.Equals(float3::zero))
	{
		movement *= (speed * dt);
		frust->Translate(movement);
	}
}

void ModuleCamera3D::rotate(float dt)
{
	if (app->input->getMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int motX = app->input->getMouseXMotion();
		int motY = app->input->getMouseYMotion();

		if (!(motX != 0 && motY != 0))
			return;

		float speed;
		if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = rotSpeed / 2;
		else
			speed = rotSpeed;

		float x = (float)-motX * speed * dt;
		float y = (float)-motY * speed * dt;

		if (app->input->getKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			orbit(x, y);
		else
			lookAt(x, y);
	}
}

void ModuleCamera3D::setPos(const float3& pos)
{
	
}

void ModuleCamera3D::orbit(float dx, float dy)
{
	//TODO: improve this: better orbit rotation and rotate with an object reference
	float3 reference = defaultCameraComp->frustum.pos + defaultCameraComp->frustum.front * distToReference;
	Quat roty(defaultCameraComp->frustum.up, dx);
	Quat rotx(defaultCameraComp->frustum.WorldRight(), dx);
	float3 vec = defaultCameraComp->frustum.pos - reference;

	vec = rotx.Transform(vec);
	vec = roty.Transform(vec);

	defaultCameraComp->frustum.pos = vec + reference;
	lookAt(reference);
}

void ModuleCamera3D::lookAt(float dx, float dy)
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

void ModuleCamera3D::zoom(float dt)
{
	if (app->input->getWheelYMotion() != 0)
	{
		Frustum* frust = &defaultCameraComp->frustum;

		float speed;
		if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = zoomSpeed / 2;
		else
			speed = zoomSpeed;

		float3 movement(float3::zero);
		float3 forw(frust->front);

		movement += (forw * app->input->getWheelYMotion());

		if (!movement.Equals(float3::zero))
		{
			movement *= (speed * dt);
			frust->Translate(movement);
		}
	}
}

//=================================================================
GameObject* ModuleCamera3D::getEditorCameraObj()const
{
	return defaultCamera;
}

Camera* ModuleCamera3D::getCamera()
{
	return defaultCameraComp;
}