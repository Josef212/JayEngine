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

	calculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(1.0f, 1.0f, 0.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);

}

ModuleCamera3D::~ModuleCamera3D()
{
	_LOG(LOG_STD, "Camera3D: Destroying.");
}

bool ModuleCamera3D::init(FileParser* conf)
{
	/*if (!defaultCamera)
		defaultCamera = app->manager->createCamera();

	if (defaultCamera)
		cameraComp = (Camera*)defaultCamera->findComponent(CAMERA)[0];

	if (!cameraComp)
		return false;
	else*/
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

	//defaultCamera = NULL;
	//cameraComp = NULL;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::update(float dt)
{
	vec3 newPos(0, 0, 0);
	float speed = 25.0f * dt;
	if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (app->input->getKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (app->input->getKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (app->input->getKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (app->input->getKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (app->input->getKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (app->input->getKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	position += newPos;
	reference += newPos;

	// Mouse motion ----------------

	if (app->input->getMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -app->input->getMouseXMotion();
		int dy = -app->input->getMouseYMotion();

		float Sensitivity = 0.25f;

		position -= reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		position = reference + Z * length(position);
	}

	// Recalculate matrix -------------
	calculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::look(const vec3 &position, const vec3 &reference, bool rotateAroundReference)
{
	this->position = position;
	this->reference = reference;

	Z = normalize(this->position - this->reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}

	calculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::lookAt( const vec3 &spot)
{
	reference = spot;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	calculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::move(const vec3 &movement)
{
	position += movement;
	reference += movement;

	calculateViewMatrix();
}

void ModuleCamera3D::setPos(const vec3 &pos)
{
	position = pos;
	lookAt(reference);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::getViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::calculateViewMatrix()
{
	viewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position), -dot(Y, position), -dot(Z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}