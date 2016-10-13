#include "Application.h"
#include "UI_Inspector.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "ModuleManager.h"
#include "ModuleWindow.h"


UI_Inspector::UI_Inspector() : UI_Comp()
{
	active = true;
}


UI_Inspector::~UI_Inspector()
{
}

void UI_Inspector::draw()
{
	int w = app->window->getWidth();
	int h = app->window->getHeight();

	GameObject* selected = app->manager->getSelected();

	ImGui::SetNextWindowPos(ImVec2(w - 250, 20));
	ImGui::SetNextWindowSize(ImVec2(250, h));

	if (ImGui::Begin("Inspector"), &active)
	{
		if (selected)
		{
			for (uint i = 0; i < selected->components.size(); ++i)
			{
				switch (selected->components[i]->type)
				{
				case TRANSFORMATION:
					drawTransformation(selected);
					break;
				case MESH:
					drawMesh(selected);
					break;
				case MATERIAL:
					drawMaterial(selected);
					break;
				}
			}
		}
		ImGui::End();
	}
}

void UI_Inspector::drawTransformation(GameObject* selected)
{
	Transform* trans = (Transform*)selected->findComponent(TRANSFORMATION);
	
	static char transName[60];
	strcpy_s(transName, 60, trans->getName());
	//Name
	if (ImGui::InputText("Name:", transName, 60))
		trans->setName(transName);
	//Active
	bool transActive = trans->isEnable();
	if (ImGui::Checkbox("Active", &transActive)) trans->switchActive();

	static float* pos = trans->getPosition();
	static float* scale = trans->getScale();
	static float* rot = trans->getEulerRot();
	//Position
	if (ImGui::DragFloat3("Position:", pos)) trans->setPosition(pos);
	//Scale
	if (ImGui::DragFloat3("Scale:", scale)) trans->setScale(scale);
	//Rotation
	if (ImGui::DragFloat3("Rotation:", rot, 0.5f, -360.f, 360.f)) trans->setRotation(rot);

	ImGui::Separator();
}

void UI_Inspector::drawMesh(GameObject* selected)
{
	Mesh* mesh = (Mesh*)selected->findComponent(MESH);

	static char meshName[60];
	strcpy_s(meshName, 60, mesh->getName());
	//Name
	if (ImGui::InputText("Name:", meshName, 60))
		mesh->setName(meshName);
	//Active
	bool meshActive = mesh->isEnable();
	if (ImGui::Checkbox("Active", &meshActive))
	{
		mesh->switchActive();
	}

	ImGui::Text("Number of vertices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->numVertices);

	ImGui::Text("Number of indices: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->numIndices);

	ImGui::Text("Number of normals: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->numNormals / 3);

	ImGui::Text("Number of texture coords: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->numTexCoords / 2);

	bool wire = mesh->renderWireframe;
	if (ImGui::Checkbox("Wireframe", &wire)) mesh->renderWireframe = wire;

	ImGui::Separator();
}

void UI_Inspector::drawMaterial(GameObject* selected)
{
	Material* mat = (Material*)selected->findComponent(MATERIAL);

	static char matName[60];
	strcpy_s(matName, 60, mat->getName());
	//Name
	if (ImGui::InputText("Name:", matName, 60)) mat->setName(matName);
	//Active
	bool metActive = mat->isEnable();
	if (ImGui::Checkbox("Active", &metActive)) mat->switchActive();
}