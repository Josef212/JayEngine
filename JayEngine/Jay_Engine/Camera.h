#ifndef __CAMERACOMP_H__
#define __CAMERACOMP_H__

#include "Component.h"
#include "Math.h"
#include "Color.h"

class Camera : public Component
{
public:
	Camera(GameObject* gObj, int id);
	virtual ~Camera();

	float getFOV()const; //Vertical FOV
	void setFOV(float vFOV); //Vertical FOV

	float getFarPlaneDist()const;
	void setFarPlaneDist(float farPlaneD);

	float getNearPlaneDist()const;
	void setNearPlaneDist(float nearPlaneD);

	float getAspectRatio()const;
	void setAspectRatio(float ratio);

	bool isCullingActive()const;
	void setCulling(bool set);

	Color getBackground();
	void getBackground(float& r, float& g, float& b, float& a);
	void setBackground(float r, float g, float b, float a);

	void move();

	//View matrix must update every frame
	float* getGLViewMatrix();
	//Projection matrix every time camera is changed like fov or window is resized
	float* getGLProjectMatrix();

private:

public:
	Color background;
	bool projectMatrixChanged = false;

private:
	Frustum frustrum;
	float nearPlaneDist = 1.f;
	float farPlaneDist = 1000.f;
	float FOV = 75.f; //Actually vertical fov
	float aspectRatio = 16 / 9;

	bool culling = false;
};


#endif // !__CAMERACOMP_H__