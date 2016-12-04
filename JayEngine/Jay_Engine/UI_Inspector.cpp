#include "Application.h"
#include "UI_Inspector.h"

#include "OpenGL.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ResourceMesh.h"

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

	ImGui::SetNextWindowPos(ImVec2(windowW - 350, 20));
	ImGui::SetNextWindowSize(ImVec2(350, windowH - 20));

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

			if (ImGui::Button("Delete.")) app->goManager->deleteGameObject(selected);

			ImGui::Separator();

			for (uint i = 0; i < selected->components.size(); ++i)
			{
				switch (selected->components[i]->type)
				{
				case TRANSFORMATION:
					drawTransformation(selected);
					break;
				case MESH:
					drawMesh(selected, (Mesh*)selected->components[i]);
					break;
				case MATERIAL:
					drawMaterial(selected, (Material*)selected->components[i]);
					break;
				case CAMERA:
					drawCamera(selected, (Camera*)selected->components[i]);
					break;
				}
			}
		}
		ImGui::End();
	}
}

void UI_Inspector::drawTransformation(GameObject* selected)
{
	Transform* trans = selected->getTransform();
	if(trans )
		trans = (Transform*)selected->findComponent(TRANSFORMATION)[0];
	
	char transName[60];
	strcpy_s(transName, 60, trans->getName());
	
	//Active
	bool transActive = trans->isEnable();
	if (ImGui::Checkbox("Transform:", &transActive)) trans->switchActive();

	ImGui::SameLine();

	if (ImGui::Button("Remove trans")) selected->removeComponent(trans);

	float* pos = trans->getPosition();
	float* scale = trans->getScale();
	float* rot = trans->getEulerRot();
	//Position
	if (ImGui::DragFloat3("Position:", pos)) trans->setPosition(pos);
	//Scale
	if (ImGui::DragFloat3("Scale:", scale)) trans->setScale(scale);
	//Rotation
	if (ImGui::DragFloat3("Rotation:", rot, 0.5f)) trans->setRotation(rot);

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

	if (ImGui::Button("Remove mesh")) selected->removeComponent(mesh);

	ImGui::Text("Number of vertices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->meshResource->numVertices);

	ImGui::Text("Number of indices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->meshResource->numIndices);

	ImGui::Text("Number of normals: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->meshResource->numNormals);

	ImGui::Text("Number of texture coords: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->meshResource->numTexCoords);

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

	if (ImGui::Button("Remove material")) selected->removeComponent(mat);

	ImGui::ColorEdit4("Color:", (float*)&mat->color, false);
	//TODO: Show all textures

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