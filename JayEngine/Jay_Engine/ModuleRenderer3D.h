#ifndef __MODULERENDERER3D__
#define __MODULERENDERER3D__

#include "Module.h"
#include "Globals.h"
#include "Light.h"

class GameObject;
class Camera;
class ResourceMesh;

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool startEnabled = true);
	~ModuleRenderer3D();

	bool Init(FileParser* conf)override;
	bool Start()override;
	update_status PreUpdate(float dt)override;
	update_status PostUpdate(float dt)override;
	bool CleanUp()override;

	void OnResize(int width, int height);

	bool GetVSync()const;
	void SetVSync(bool vsync);

	void DrawGameObject(GameObject* obj);

	void SetActiveCamera(Camera* activeCamera);
	Camera* GetActiveCamera()const;

	void OnGlobalEvent(const Event& e);

private:
	void UpdateProjectionMat(Camera* cam);
	void DrawWireframe(ResourceMesh* resMesh, bool selected);
	void DrawNormals(ResourceMesh* resMesh);

public:
	Light lights[MAX_LIGHTS];
	bool showGrid = true;
	bool editorShowGrid = showGrid;

private:
	SDL_GLContext context;
	bool vsync = false;
	Camera* activeCamera = nullptr;

};

#endif // !__MODULERENDERER3D__