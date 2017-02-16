#ifndef __CAMERA__COMP__
#define __CAMERA__COMP__

#include "Component.h"
#include "Math.h"
#include "Color.h"

class Transform;
class FileParser;

class Camera : public Component
{
public:
	Camera(GameObject* gObj, int id);
	virtual ~Camera();

	float GetFOV()const; //Vertical FOV
	void SetFOV(float vFOV); //Vertical FOV

	float GetFarPlaneDist()const;
	void SetFarPlaneDist(float farPlaneD);

	float GetNearPlaneDist()const;
	void SetNearPlaneDist(float nearPlaneD);

	float GetAspectRatio()const;
	void SetAspectRatio(float ratio);

	bool IsCullingActive()const;
	void SetCulling(bool set);

	Color GetBackground();
	void GetBackground(float& r, float& g, float& b, float& a);
	void SetBackground(float r, float g, float b, float a);

	//View matrix must update every frame
	float* GetGLViewMatrix();
	//Projection matrix every time camera is changed like fov or window is resized
	float* GetGLProjectMatrix();

	void LookAt(const float3 spot);
	void Look(const float3 spot, const float3 pos);

	void OnTransformUpdate(Transform* trans)override;

	void DebugDraw()override;

	bool SaveCMP(FileParser& sect)override;
	bool LoadCMP(FileParser& sect)override;

private:

public:
	Color background;
	bool projectMatrixChanged = false;
	Frustum frustum;

private:
	float nearPlaneDist = 1.f;
	float farPlaneDist = 400.f;
	float FOV = 75.f; //Actually vertical fov
	float aspectRatio = 16 / 9;

	bool culling = false;
};


#endif // !__CAMERA__COMP__