#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool startEnabled = true);
	~ModuleRenderer3D();

	bool init();
	update_status preUpdate(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	void onResize(int width, int height);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;
};

#endif // !__MODULERENDERER3D_H__