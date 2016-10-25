#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

#include "DrawDebug.h"

//MathGeolib frustum: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php

Camera::Camera(GameObject* gObj, int id) : Component(gObj, id)
{
	type = CAMERA;
	name.assign("Camera");

	frustum.pos = float3(0.f, 0.f, 0.f);
	frustum.front = float3(0.f, 0.f, 1.f);
	frustum.up = float3(0.f, 1.f, 0.f);

	frustum.nearPlaneDistance = nearPlaneDist;
	frustum.farPlaneDistance = farPlaneDist;
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
		frustum.horizontalFov = DEGTORAD * FOV;
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
		frustum.farPlaneDistance = farPlaneDist;
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
		frustum.nearPlaneDistance = nearPlaneDist;
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
		frustum.horizontalFov = 2 * atan(tan(FOV / 2) * aspectRatio);
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

	frustum.pos = (float3)trans->getPosition();
	frustum.front = mat.WorldZ();
	frustum.up = mat.WorldY();
}

float* Camera::getGLViewMatrix()
{
	float4x4 ret = frustum.ViewMatrix();
	return (float*)ret.Transposed().v;
}

float* Camera::getGLProjectMatrix()
{
	float4x4 ret = frustum.ProjectionMatrix();
	return (float*)ret.Transposed().v;
}

void Camera::debugDraw()
{
	drawFrustumDebug(frustum);
}

//TMP
void Camera::update(float dt)
{
	Transform* trans = object->getTransform();
	if (!trans)
		return;
	float4x4 mat = trans->getTransformMatrix();

	frustum.pos = (float3)trans->getPosition();
	frustum.front = mat.WorldZ();
	frustum.up = mat.WorldY();
}