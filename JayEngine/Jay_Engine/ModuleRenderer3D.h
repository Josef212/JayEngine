#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

class GameObject;
class Camera;

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool startEnabled = true);
	~ModuleRenderer3D();

	bool init(FileParser* conf);
	bool start();
	update_status preUpdate(float dt);
	update_status postUpdate(float dt);
	bool cleanUp();

	void onResize(int width, int height);

	bool getVSync()const;
	void setVSync(bool vsync);

	void drawGameObject(GameObject* obj);
	void drawWireframe(bool selected);

	void setActiveCamera(Camera* activeCamera);
	Camera* getActiveCamera()const;

	void onGlobalEvent(const Event& e);

private:
	void updateProjectionMat(Camera* cam);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;
	bool showGrid = true;
	bool editorShowGrid = showGrid;

private:
	bool vsync;
	Camera* activeCamera = NULL;

};

#endif // !__MODULERENDERER3D_H__