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
	ModuleRenderer3D(bool startEnabled = true);
	~ModuleRenderer3D();

	bool init();
	bool start();
	update_status preUpdate(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	void onResize(int width, int height);

	bool getVSync()const;
	void setVSync(bool vsync);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;

private:
	bool vsync;
	bool showGrid = true;

	//TMP
	uint vArray = 0;
};

#endif // !__MODULERENDERER3D_H__