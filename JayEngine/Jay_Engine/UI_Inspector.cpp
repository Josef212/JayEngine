#include "Application.h"
#include "UI_Inspector.h"

#include "OpenGL.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ModuleResourceManager.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceShader.h"

#include "ModuleGOManager.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

//TMP!!!
#include "FileParser.h"
#include "ModuleFileSystem.h"

UI_Inspector::UI_Inspector(bool startEnalbed) : UI_Panel(startEnalbed)
{
}


UI_Inspector::~UI_Inspector()
{
}

void UI_Inspector::Draw()
{
	int w = app->window->GetWidth();
	int h = app->window->GetHeight();

	//ImGui::SetNextWindowPos(ImVec2(1570, 20));
	//ImGui::SetNextWindowSize(ImVec2(350, 810));

	ImGui::SetNextWindowPos(ImVec2(w*0.8, 20));
	ImGui::SetNextWindowSize(ImVec2(w*0.2, (h - 20)*0.8 - 55));

	GameObject* selected = app->goManager->GetSelected();

	ImGui::Begin("Inspector", &active);
	{
		if (selected)
		{
			char name[128];
			sprintf_s(name, 128, selected->GetName());
			if (ImGui::InputText("Name", name, 128)) selected->SetName(name);

			ImGui::Separator();

			bool showAABB = selected->drawEnclosingAABB;
			if (ImGui::Checkbox("Show enclosing AABB", &showAABB)) app->goManager->MakeGOShowAABoxRec(selected, showAABB);
			ImGui::SameLine();
			bool showOBB = selected->drawOrientedBox;
			if (ImGui::Checkbox("Show oriented box", &showOBB)) app->goManager->MakeGOShowOBoxRec(selected, showOBB);

			if (ImGui::Button("Delete.")) selected->Remove();

			ImGui::Separator();

			for (uint i = 0; i < selected->components.size(); ++i)
			{
				Component* cmp = selected->components[i];
				if (cmp)
				{
					switch (cmp->GetType())
					{
					case CMP_TRANSFORMATION:
						DrawTransformation(selected);
						break;
					case CMP_MESH:
						DrawMesh(selected, (Mesh*)cmp);
						break;
					case CMP_MATERIAL:
						DrawMaterial(selected, (Material*)cmp);
						break;
					case CMP_CAMERA:
						DrawCamera(selected, (Camera*)cmp);
						break;
					}
				}
			}
		}
		ImGui::End();
	}
}

void UI_Inspector::DrawTransformation(GameObject* selected)
{
	Transform* trans = selected->transform;
	if(!trans)
		trans = (Transform*)selected->GetComponents(CMP_TRANSFORMATION)[0];
	
	if (!trans)
		return;
	
	//Active
	bool transActive = trans->IsActive();
	if (ImGui::Checkbox("Transform:", &transActive)) trans->SwitchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove trans")) trans->Destroy();

	if (ImGui::Button("Reset transform"))
	{
		trans->SetLocalPosition(float3::zero);
		trans->SetLocalScale(float3::one);
		trans->SetLocalRotation(Quat::identity);
	}

	float3 pos = trans->GetLocalPosition();
	float3 scale = trans->GetLocalScale();
	float3 rot = trans->GetLocalRotation();
	rot.x *= RADTODEG;
	rot.y *= RADTODEG;
	rot.z *= RADTODEG;

	//Position
	if (ImGui::DragFloat3("Position:", (float*)&pos, 0.25f)) trans->SetLocalPosition(pos);
	//Scale
	if (ImGui::DragFloat3("Scale:", (float*)&scale)) trans->SetLocalScale(scale);
	//Rotation
	if (ImGui::SliderAngle3("Rotation:", (float*)&rot, 0.5f)) trans->SetLocalRotation(rot);

	ImGui::Separator();
}

void UI_Inspector::DrawMesh(GameObject* selected, Mesh* mesh)
{

	//Active
	bool meshActive = mesh->IsActive();
	if (ImGui::Checkbox("Mesh:", &meshActive))
		mesh->SwitchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove mesh")) mesh->Destroy();

	static uint nVertices = 0;
	static uint nIndices = 0;
	static uint nNormals = 0;
	static uint nTexCoords = 0;

	if (mesh->meshResource)
	{
		nVertices = mesh->meshResource->numVertices;
		nIndices = mesh->meshResource->numIndices;
		nNormals = mesh->meshResource->numNormals;
		nTexCoords = mesh->meshResource->numTexCoords;

		ImGui::Text("File: ");
		ImGui::SameLine();
		ImGui::TextColored(ImColor(255, 153, 51), "%s.", mesh->meshResource->exportedFile.c_str());
	}

	ImGui::Text("Number of vertices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", nVertices);

	ImGui::Text("Number of indices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d",nIndices);

	ImGui::Text("Number of normals: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", nNormals);

	ImGui::Text("Number of texture coords: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", nTexCoords);

	bool wire = mesh->renderWireframe;
	if (ImGui::Checkbox("Wireframe", &wire)) mesh->renderWireframe = wire;

	bool normals = mesh->renderNormals;
	if (ImGui::Checkbox("Normals", &normals)) mesh->renderNormals = normals;

	ImGui::Separator();
}

void UI_Inspector::DrawMaterial(GameObject* selected, Material* mat)
{
	//Active
	bool metActive = mat->IsActive();
	if (ImGui::Checkbox("Material:", &metActive)) mat->SwitchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove material")) mat->Destroy();

	ImGui::ColorEdit4("Color:", (float*)&mat->color, false);
	//TODO: Show all textures

	ImGui::Text("Shader id(0 or %d if using default):", app->resourceManager->GetDefaultShader()->shaderID);
	ImGui::SameLine();
	int sh = 0;
	if (mat->shaderResource)
		sh = mat->shaderResource->shaderID;

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", sh);

	if (sh != 0)
	{
		if (ImGui::Button("Set default shader"))
			mat->shaderResource = app->resourceManager->GetDefaultShader();
	}

	ImGui::Separator();
}

void UI_Inspector::DrawCamera(GameObject* selected, Camera* cam)
{
	if (cam)
	{
		ImGui::Text("Camera:");
		ImGui::ColorEdit4("Background:", (float*)&cam->background, false);

		float nearP = cam->GetNearPlaneDist();
		float farP = cam->GetFarPlaneDist();
		float fov = cam->GetFOV();

		if (ImGui::DragFloat("Near plane:", &nearP)) cam->SetNearPlaneDist(nearP);
		if (ImGui::DragFloat("Far plane:", &farP))cam->SetFarPlaneDist(farP);
		if (ImGui::DragFloat("Field of view:", &fov))cam->SetFOV(fov);

		bool culling = cam->IsCullingActive();
		if (ImGui::Checkbox("Culling", &culling))cam->SetCulling(culling);

		if (ImGui::Button("Make this active."))app->renderer3D->SetActiveCamera(cam);
		if (app->renderer3D->GetActiveCamera() == cam)
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "This is the active camera!");

		ImGui::Separator();
		
	}
}