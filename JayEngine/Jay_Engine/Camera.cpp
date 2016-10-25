#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

//MathGeolib frustrum: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php

Camera::Camera(GameObject* gObj, int id) : Component(gObj, id)
{
	type = CAMERA;
	name.assign("Camera");

	frustrum.pos = float3(0.f, 0.f, 0.f);
	frustrum.front = float3(0.f, 0.f, 1.f);
	frustrum.up = float3(0.f, 1.f, 0.f);

	frustrum.nearPlaneDistance = nearPlaneDist;
	frustrum.farPlaneDistance = farPlaneDist;
	setFOV(FOV);
}

Camera::~Camera()
{
}


float Camera::getFOV()const //Vertical FOV
{
	return FOV;
}

void Camera::setFOV(float vFOV) //Vertical FOV
{
	if (vFOV > 0.f)
	{
		//Will set manually aspect ratio (h fov) cause can use SetVerticalFovAndAspectRatio function
		FOV = vFOV;
		frustrum.horizontalFov = DEGTORAD * FOV;
		setAspectRatio(aspectRatio);
	}
}

float Camera::getFarPlaneDist()const
{
	return farPlaneDist;
}

void Camera::setFarPlaneDist(float farPlaneD)
{
	if (farPlaneD > 0.f && farPlaneD > nearPlaneDist)
	{
		farPlaneDist = farPlaneD;
		frustrum.farPlaneDistance = farPlaneDist;
		projectMatrixChanged = true;
	}
}

float Camera::getNearPlaneDist()const
{
	return nearPlaneDist;
}

void Camera::setNearPlaneDist(float nearPlaneD)
{
	if (nearPlaneD > 0.f && nearPlaneD < farPlaneDist)
	{
		nearPlaneDist = nearPlaneD;
		frustrum.nearPlaneDistance = nearPlaneDist;
		projectMatrixChanged = true;
	}
}

float Camera::getAspectRatio()const
{
	return aspectRatio;
}

void Camera::setAspectRatio(float ratio)
{
	if (ratio > 0)
	{
		aspectRatio = ratio;
		frustrum.horizontalFov = 2 * atan(tan(FOV / 2) * aspectRatio);
		projectMatrixChanged = true;
	}
}

bool Camera::isCullingActive()const
{
	return culling;
}

void Camera::setCulling(bool set)
{
	culling = set;
}

Color Camera::getBackground()
{
	return background;
}

void Camera::getBackground(float& r, float& g, float& b, float& a)
{
	r = background.r;
	g = background.g;
	b = background.b;
	a = background.a;
}

void Camera::setBackground(float r, float g, float b, float a)
{
	background.Set(r, g, b, a);
}

void Camera::move()
{
	Transform* trans = object->getTransform();
	if (!trans)
		return;
	float4x4 mat = trans->getTransformMatrix();

	frustrum.pos = (float3)trans->getPosition();
	frustrum.front = mat.WorldZ();
	frustrum.up = mat.WorldY();
}

float* Camera::getGLViewMatrix()
{
	float4x4 ret = frustrum.ViewMatrix();
	return (float*)ret.Transposed().v;
}

float* Camera::getGLProjectMatrix()
{
	float4x4 ret = frustrum.ProjectionMatrix();
	return (float*)ret.Transposed().v;
}