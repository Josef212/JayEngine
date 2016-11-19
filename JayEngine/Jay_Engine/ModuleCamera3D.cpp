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
	/*vec3 newPos(0, 0, 0);
	float speed = 25.0f * dt;
	if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (app->input->getKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (app->input->getKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (app->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (app->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (app->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (app->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//position += newPos;
	//reference += newPos;

	// Mouse motion ----------------

	if (app->input->getMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -app->input->getMouseXMotion();
		int dy = -app->input->getMouseYMotion();

		float Sensitivity = 0.25f;

		//position -= reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			//X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			//Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			//Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			//Y = rotate(Y, DeltaY, X);
			//Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		position = reference + Z * length(position);
	}

	// Recalculate matrix -------------
	calculateViewMatrix();*/

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
	
}

void ModuleCamera3D::setPos(const float3& pos)
{
	
}

void ModuleCamera3D::orbit(const float3 ref, float dt)
{

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