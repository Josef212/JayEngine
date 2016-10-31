#include "Application.h"
#include "UI_Inspector.h"

#include "OpenGL.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ModuleManager.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"


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

	GameObject* selected = app->manager->getSelected();

	ImGui::SetNextWindowPos(ImVec2(windowW - 300, 20));
	ImGui::SetNextWindowSize(ImVec2(300, windowH - 20));

	ImGui::Begin("Inspector", &active);
	{
		ImGui::Checkbox("ShowGrid", &app->renderer3D->showGrid);
		if (selected)
		{
			char name[128];
			sprintf_s(name, 128, selected->getName());
			if (ImGui::InputText("Name", name, 128)) selected->setName(name);

			ImGui::Separator();

			bool showAABB = selected->drawEnclosingAABB;
			if (ImGui::Checkbox("Show enclosing AABB", &showAABB)) selected->drawEnclosingAABB = showAABB;
			ImGui::SameLine();
			bool showOBB = selected->drawOrientedBox;
			if (ImGui::Checkbox("Show oriented box", &showOBB)) selected->drawOrientedBox = showOBB;

			if (ImGui::Button("Delete.")) app->manager->deleteGameObject(selected);

			ImGui::Separator();

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
				case CAMERA:
					drawCamera(selected);
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

void UI_Inspector::drawMesh(GameObject* selected) //TODO: must iterate all meshes to show components info in inspector
{
	Mesh* mesh = (Mesh*)selected->findComponent(MESH)[0];

	char meshName[60];
	strcpy_s(meshName, 60, mesh->getName());

	//Active
	bool meshActive = mesh->isEnable();
	if (ImGui::Checkbox("Mesh:", &meshActive))
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

	ImGui::Text("Texture ID: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mesh->idTexture);

	bool wire = mesh->renderWireframe;
	if (ImGui::Checkbox("Wireframe", &wire)) mesh->renderWireframe = wire;

	bool normals = mesh->renderNormals;
	if (ImGui::Checkbox("Normals", &normals)) mesh->renderNormals = normals;

	ImGui::Separator();
}

void UI_Inspector::drawMaterial(GameObject* selected) //TODO: must iterate all materials to show components info in inspector
{
	Material* mat = (Material*)selected->findComponent(MATERIAL)[0];

	char matName[60];
	strcpy_s(matName, 60, mat->getName());

	//Active
	bool metActive = mat->isEnable();
	if (ImGui::Checkbox("Material:", &metActive)) mat->switchActive();

	ImGui::Text("Textures vec size: ");
	ImGui::SameLine();
	ImGui::TextColored(ImColor(255, 153, 51), "%d", mat->getTexxturesSize());
	
	ImGui::ColorEdit4("Color:", (float*)&mat->color, false);
	
	for (std::map<std::string, int>::iterator it = mat->paths.begin(); it != mat->paths.end(); ++it)
	{
		ImGui::Text("%s", (*it).first.c_str());

	}

	std::map<std::string, int>::iterator it = mat->paths.begin();
	for (uint i = 0; i < mat->textures.size() && it != mat->paths.end(); ++i, ++it)
	{
		if (ImGui::TreeNode("%s", (*it).first.c_str()))
		{
			uint j = mat->textures[i];
			glBindTexture(GL_TEXTURE_2D, j);
			ImTextureID texture = (void*)j;
			ImGui::Image(texture, ImVec2(windowW, 300));

			ImGui::TreePop();
		}
	}

	ImGui::Separator();
}

void UI_Inspector::drawCamera(GameObject* selected)
{
	Camera* cam = (Camera*)selected->findComponent(CAMERA)[0];

	if (!cam)
		return;

	ImGui::ColorEdit4("Background:", (float*)&cam->background, false);

	float nearP = cam->getNearPlaneDist();
	float farP = cam->getFarPlaneDist();
	float fov = cam->getFOV();
	
	if(ImGui::DragFloat("Near plane:", &nearP)) cam->setNearPlaneDist(nearP);
	if(ImGui::DragFloat("Far plane:", &farP))cam->setFarPlaneDist(farP);
	if(ImGui::DragFloat("Field of view:", &fov))cam->setFOV(fov);

	static int item = 0;
	if (ImGui::Combo("AspectRatio", &item, " 16:9\0 16:10\0 5:4\0 4:3\0\0"))
	{
		switch (item)
		{
			case 0:
			{
				cam->setAspectRatio(16 / 9);
			}
			break;
			case 1:
			{
				cam->setAspectRatio(16 / 10);
			}
			break;
			case 2:
			{
				cam->setAspectRatio(5/4);
			}
			break;
			case 3:
			{
				cam->setAspectRatio(4/3);
			}
			break;
		}
	}

	bool culling = cam->isCullingActive();
	if (ImGui::Checkbox("Culling", &culling))cam->setCulling(culling);

	if(ImGui::Button("Make this active."));

	ImGui::Separator();
}