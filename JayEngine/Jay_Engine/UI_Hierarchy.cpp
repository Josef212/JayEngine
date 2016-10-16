#include "Application.h"
#include "UI_Hierarchy.h"

#include "ModuleManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "ModuleWindow.h"

UI_Hierarchy::UI_Hierarchy() : UI_Comp()
{
	active = true;
}


UI_Hierarchy::~UI_Hierarchy()
{
}

void UI_Hierarchy::draw()
{
	int w = app->window->getWidth();
	int h = app->window->getHeight();
	
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	//ImGui::SetNextWindowSize(ImVec2(200, h / 2));
	ImGui::Begin("Hierarchy", &active);
	{
		GameObject* root = app->manager->getSceneroot();
		GameObject* selected = app->manager->getSelected();

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (!selected)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx("Scene", nodeFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.

			if (ImGui::IsItemClicked())
				app->manager->select(NULL);

			if (root)
			{
				for (uint i = 0; i < root->childrens.size(); ++i)
					hierarchyRecursive(root->childrens[i], selected);
			}

			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
		ImGui::End();
	}
}

void UI_Hierarchy::hierarchyRecursive(GameObject* node, GameObject* selected)
{
	ImGuiTreeNodeFlags nodeFlags = 0;
	if (node == selected)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	if (node->childrens.size() > 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
		nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
	}
	else
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node->getName(), nodeFlags))
	{
		if (ImGui::IsItemClicked())
			app->manager->select(node);

		for (uint i = 0; i < node->childrens.size(); ++i)
		{
			hierarchyRecursive(node->childrens[i], selected);
		}
		ImGui::TreePop();
	}
}