#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

#include "Application.h"
#include "FileParser.h"

#include "DrawDebug.h"

//MathGeolib frustum: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php

Camera::Camera(GameObject* gObj, int id) : Component(gObj, id)
{
	type = CAMERA;

	name.assign("Camera");

	frustum.type = PerspectiveFrustum;

	frustum.pos = float3(0.f, 0.f, 0.f);
	frustum.front = float3(0.f, 0.f, 1.f);
	frustum.up = float3(0.f, 1.f, 0.f);

	frustum.nearPlaneDistance = nearPlaneDist;
	frustum.farPlaneDistance = farPlaneDist;
	setFOV(FOV);
	setAspectRatio(aspectRatio);
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
		frustum.verticalFov = DEGTORAD * FOV;
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
		frustum.horizontalFov = 2.f * atan(tan(frustum.verticalFov * 0.5f) * aspectRatio);
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

void Camera::onTransformUpdate(Transform* trans)
{
	if (!trans)
		return;

	float4x4 mat = trans->getGlobalTransform();

	frustum.pos = mat.TranslatePart();
	frustum.front = mat.WorldZ();
	frustum.up = mat.WorldY();
}

float* Camera::getGLViewMatrix()
{
	static float4x4 ret;
	ret = frustum.ViewMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

float* Camera::getGLProjectMatrix()
{
	static float4x4 ret;
	ret = frustum.ProjectionMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

void Camera::debugDraw()
{
	if(app->debug)
		drawFrustumDebug(frustum);
}

void Camera::lookAt(const float3 spot)
{
	float3 direction = spot - frustum.pos;
	float3x3 mat = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);

	frustum.front = mat.MulDir(frustum.front).Normalized();
	frustum.up = mat.MulDir(frustum.up).Normalized();
}

void Camera::look(const float3 spot, const float3 pos)
{
	frustum.pos = pos;
	lookAt(spot);
	projectMatrixChanged = true;
}

bool Camera::saveCMP(FileParser& sect)
{
	bool ret = true;

	sect.addInt("comp_type", (int)type);
	sect.addBool("active", active);
	sect.addInt("UUID", id);
	sect.addInt("go_UUID", object->getGOId());

	sect.addFloat("near_plane", nearPlaneDist);
	sect.addFloat("far_plane", farPlaneDist);
	sect.addFloat("fov", FOV);
	sect.addFloat("aspect_ratio", aspectRatio);
	//TODO: Front?? Right???

	sect.addFloat3("cam_pos", frustum.pos);
	sect.addColor("bg_color", background);

	return ret;
}

bool Camera::loadCMP(FileParser& sect)
{
	bool ret = true;

	active = sect.getBool("active", true);
	id = sect.getInt("UUID", 0);

	nearPlaneDist = sect.getFloat("near_plane", 0.f);
	farPlaneDist = sect.getFloat("far_plane", 0.f);
	FOV = sect.getFloat("fov", 0.f);
	setAspectRatio(sect.getFloat("aspect_ratio", 1.3f));

	frustum.pos = sect.getFloat3("cam_pos", float3::zero);
	background = sect.getColor("bg_color", Black);

	return ret;
}