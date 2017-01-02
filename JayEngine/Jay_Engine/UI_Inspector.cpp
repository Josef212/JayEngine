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

UI_Inspector::UI_Inspector() : UI_Comp()
{
	active = true;
}


UI_Inspector::~UI_Inspector()
{
}

void UI_Inspector::draw()
{
	ImGuiIO& io = ImGui::GetIO();
	windowW = io.DisplaySize.x;
	windowH = io.DisplaySize.y;

	GameObject* selected = app->goManager->getSelected();

	ImGui::SetNextWindowPos(ImVec2(1570, 20));
	ImGui::SetNextWindowSize(ImVec2(350, 810));

	ImGui::Begin("Inspector", &active);
	{
		if (selected)
		{
			char name[128];
			sprintf_s(name, 128, selected->getName());
			if (ImGui::InputText("Name", name, 128)) selected->setName(name);

			ImGui::Separator();

			bool showAABB = selected->drawEnclosingAABB;
			if (ImGui::Checkbox("Show enclosing AABB", &showAABB)) app->goManager->makeGOShowAABoxRec(selected, showAABB);
			ImGui::SameLine();
			bool showOBB = selected->drawOrientedBox;
			if (ImGui::Checkbox("Show oriented box", &showOBB)) app->goManager->makeGOShowOBoxRec(selected, showOBB);

			if (ImGui::Button("Delete.")) selected->remove();

			ImGui::Separator();

			for (uint i = 0; i < selected->components.size(); ++i)
			{
				Component* cmp = selected->components[i];
				if (cmp)
				{
					switch (cmp->type)
					{
					case TRANSFORMATION:
						drawTransformation(selected);
						break;
					case MESH:
						drawMesh(selected, (Mesh*)cmp);
						break;
					case MATERIAL:
						drawMaterial(selected, (Material*)cmp);
						break;
					case CAMERA:
						drawCamera(selected, (Camera*)cmp);
						break;
					}
				}
			}
		}
		ImGui::End();
	}
}

void UI_Inspector::drawTransformation(GameObject* selected)
{
	Transform* trans = selected->transform;
	if(!trans)
		trans = (Transform*)selected->findComponent(TRANSFORMATION)[0];
	
	if (!trans)
		return;

	char transName[60];
	strcpy_s(transName, 60, trans->getName());
	
	//Active
	bool transActive = trans->isEnable();
	if (ImGui::Checkbox("Transform:", &transActive)) trans->switchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove trans")) trans->remove();

	if (ImGui::Button("Reset transform"))
	{
		trans->setLocalPosition(float3::zero);
		trans->setLocalScale(float3::one);
		trans->setLocalRotation(Quat::identity);
	}

	float3 pos = trans->getLocalPosition();
	float3 scale = trans->getLocalScale();
	float3 rot = trans->getLocalRotation();
	rot.x *= RADTODEG;
	rot.y *= RADTODEG;
	rot.z *= RADTODEG;

	//Position
	if (ImGui::DragFloat3("Position:", (float*)&pos, 0.25f)) trans->setLocalPosition(pos);
	//Scale
	if (ImGui::DragFloat3("Scale:", (float*)&scale)) trans->setLocalScale(scale);
	//Rotation
	if (ImGui::DragFloat3("Rotation:", (float*)&rot, 0.5f)) trans->setLocalRotation(rot);

	ImGui::Separator();
}

void UI_Inspector::drawMesh(GameObject* selected, Mesh* mesh)
{
	char meshName[60];
	strcpy_s(meshName, 60, mesh->getName());

	//Active
	bool meshActive = mesh->isEnable();
	if (ImGui::Checkbox("Mesh:", &meshActive))
		mesh->switchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove mesh")) mesh->remove();

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

void UI_Inspector::drawMaterial(GameObject* selected, Material* mat)
{
	char matName[60];
	strcpy_s(matName, 60, mat->getName());

	//Active
	bool metActive = mat->isEnable();
	if (ImGui::Checkbox("Material:", &metActive)) mat->switchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove material")) mat->remove();

	ImGui::ColorEdit4("Color:", (float*)&mat->color, false);
	//TODO: Show all textures

	ImGui::Text("Shader id(0 or %d if using default):", app->resourceManager->getDefaultShader()->shaderID);
	ImGui::SameLine();
	int sh = 0;
	if (mat->shaderResource)
		sh = mat->shaderResource->shaderID;

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", sh);

	if (sh != 0)
	{
		if (ImGui::Button("Set default shader"))
			mat->shaderResource = app->resourceManager->getDefaultShader();
	}

	ImGui::Separator();
}

void UI_Inspector::drawCamera(GameObject* selected, Camera* cam)
{
	if (cam)
	{
		ImGui::Text("Camera:");
		ImGui::ColorEdit4("Background:", (float*)&cam->background, false);

		float nearP = cam->getNearPlaneDist();
		float farP = cam->getFarPlaneDist();
		float fov = cam->getFOV();

		if (ImGui::DragFloat("Near plane:", &nearP)) cam->setNearPlaneDist(nearP);
		if (ImGui::DragFloat("Far plane:", &farP))cam->setFarPlaneDist(farP);
		if (ImGui::DragFloat("Field of view:", &fov))cam->setFOV(fov);

		bool culling = cam->isCullingActive();
		if (ImGui::Checkbox("Culling", &culling))cam->setCulling(culling);

		if (ImGui::Button("Make this active."))app->renderer3D->setActiveCamera(cam);
		if (app->renderer3D->getActiveCamera() == cam)
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "This is the active camera!");

		ImGui::Separator();
		
	}
}