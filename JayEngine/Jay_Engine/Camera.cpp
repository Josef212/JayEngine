#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

#include "Application.h"
#include "FileParser.h"

#include "ModuleRenderer3D.h"

#include "DrawDebug.h"

//MathGeolib frustum: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php

Camera::Camera(GameObject* gObj) : Component(gObj)
{
	type = CMP_CAMERA;

	frustum.type = PerspectiveFrustum;

	frustum.pos = float3(0.f, 0.f, 0.f);
	frustum.front = float3(0.f, 0.f, 1.f);
	frustum.up = float3(0.f, 1.f, 0.f);

	frustum.nearPlaneDistance = nearPlaneDist;
	frustum.farPlaneDistance = farPlaneDist;
	SetFOV(FOV);
	SetAspectRatio(aspectRatio);
}

Camera::~Camera()
{
	if (app->renderer3D->GetActiveCamera() == this)
		app->renderer3D->SetActiveCamera(nullptr);
}


float Camera::GetFOV()const //Vertical FOV
{
	return FOV;
}

void Camera::SetFOV(float vFOV) //Vertical FOV
{
	if (vFOV > 0.f)
	{
		//Will set manually aspect ratio (h fov) cause can use SetVerticalFovAndAspectRatio function
		FOV = vFOV;
		frustum.verticalFov = DEGTORAD * FOV;
		SetAspectRatio(aspectRatio);
	}
}

float Camera::GetFarPlaneDist()const
{
	return farPlaneDist;
}

void Camera::SetFarPlaneDist(float farPlaneD)
{
	if (farPlaneD > 0.f && farPlaneD > nearPlaneDist)
	{
		farPlaneDist = farPlaneD;
		frustum.farPlaneDistance = farPlaneDist;
		projectMatrixChanged = true;
	}
}

float Camera::GetNearPlaneDist()const
{
	return nearPlaneDist;
}

void Camera::SetNearPlaneDist(float nearPlaneD)
{
	if (nearPlaneD > 0.f && nearPlaneD < farPlaneDist)
	{
		nearPlaneDist = nearPlaneD;
		frustum.nearPlaneDistance = nearPlaneDist;
		projectMatrixChanged = true;
	}
}

float Camera::GetAspectRatio()const
{
	return aspectRatio;
}

void Camera::SetAspectRatio(float ratio)
{
	if (ratio > 0)
	{
		aspectRatio = ratio;
		frustum.horizontalFov = 2.f * atan(tan(frustum.verticalFov * 0.5f) * aspectRatio);
		projectMatrixChanged = true;
	}
}

bool Camera::IsCullingActive()const
{
	return culling;
}

void Camera::SetCulling(bool set)
{
	culling = set;
}

Color Camera::GetBackground()
{
	return background;
}

void Camera::GetBackground(float& r, float& g, float& b, float& a)
{
	r = background.r;
	g = background.g;
	b = background.b;
	a = background.a;
}

void Camera::SetBackground(float r, float g, float b, float a)
{
	background.Set(r, g, b, a);
}

void Camera::OnTransformUpdate(Transform* trans)
{
	if (!trans)
		return;

	float4x4 mat = trans->GetGlobalTransform();

	frustum.pos = mat.TranslatePart();
	frustum.front = mat.WorldZ();
	frustum.up = mat.WorldY();
}

float* Camera::GetGLViewMatrix()
{
	static float4x4 ret;
	ret = frustum.ViewMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

float* Camera::GetGLProjectMatrix()
{
	static float4x4 ret;
	ret = frustum.ProjectionMatrix();
	ret.Transpose();
	return (float*)ret.v;
}

void Camera::OnDebugDraw()
{
	if(app->debug)
		DrawFrustumDebug(frustum);
}

void Camera::LookAt(const float3 spot)
{
	float3 direction = spot - frustum.pos;
	float3x3 mat = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);

	frustum.front = mat.MulDir(frustum.front).Normalized();
	frustum.up = mat.MulDir(frustum.up).Normalized();
}

void Camera::Look(const float3 spot, const float3 pos)
{
	frustum.pos = pos;
	LookAt(spot);
	projectMatrixChanged = true;
}

bool Camera::SaveCMP(FileParser& sect)const
{
	bool ret = true;

	sect.AddInt("comp_type", (int)type);
	sect.AddBool("active", selfActive);
	sect.AddInt("go_UUID", object->GetGOId());

	sect.AddFloat("near_plane", nearPlaneDist);
	sect.AddFloat("far_plane", farPlaneDist);
	sect.AddFloat("fov", FOV);
	sect.AddFloat("aspect_ratio", aspectRatio);

	sect.AddFloat3("cam_pos", frustum.pos);
	sect.AddColor("bg_color", background);

	if (app->renderer3D->GetActiveCamera() == this)
		sect.AddBool("is_active_cam", true);

	return ret;
}

bool Camera::LoadCMP(FileParser* sect)
{
	bool ret = true;

	selfActive = sect->GetBool("active", true);

	nearPlaneDist = sect->GetFloat("near_plane", 0.f);
	farPlaneDist = sect->GetFloat("far_plane", 0.f);
	FOV = sect->GetFloat("fov", 0.f);
	SetAspectRatio(sect->GetFloat("aspect_ratio", 1.3f));

	frustum.pos = sect->GetFloat3("cam_pos", float3::zero);
	background = sect->GetColor("bg_color", Black);

	if (sect->GetBool("is_active_cam", false))
		app->renderer3D->SetActiveCamera(this);

	return ret;
}