#include "Application.h"
#include "UI_Hierarchy.h"

#include "ModuleGOManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "ModuleWindow.h"

UI_Hierarchy::UI_Hierarchy(bool startEnalbed) : UI_Panel(startEnalbed)
{
}


UI_Hierarchy::~UI_Hierarchy()
{
}

void UI_Hierarchy::Draw()
{
	int w = app->window->GetWidth();
	int h = app->window->GetHeight();
	
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(360, 560));

	ImGui::Begin("Hierarchy", &active);
	{
		GameObject* root = app->goManager->GetSceneroot();
		GameObject* selected = app->goManager->GetSelected();

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (!selected)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::TreeNodeEx("Scene", nodeFlags))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.

			if (ImGui::IsItemClicked())
				app->goManager->Select(nullptr);

			if (root)
			{
				for (uint i = 0; i < root->childrens.size(); ++i)
					HierarchyRecursive(root->childrens[i], selected);
			}

			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
		ImGui::End();
	}
}

void UI_Hierarchy::HierarchyRecursive(GameObject* node, GameObject* selected)
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

	if (ImGui::TreeNodeEx(node->GetName(), nodeFlags))
	{
		if (ImGui::IsItemClicked())
			app->goManager->Select(node);

		for (uint i = 0; i < node->childrens.size(); ++i)
		{
			HierarchyRecursive(node->childrens[i], selected);
		}
		ImGui::TreePop();
	}
}