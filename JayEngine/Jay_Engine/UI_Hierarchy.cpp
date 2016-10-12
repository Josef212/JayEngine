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
	ImGui::SetNextWindowSize(ImVec2(200, h / 2));
	if (ImGui::Begin("Hierarchy"), &active)
	{

		if (ImGui::TreeNode("Scene"))
		{
			GameObject* root = app->manager->getSceneroot();
			GameObject* selected = app->manager->getSelected();

			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.

			if (root)
			{
				hierarchyRecursive(root, selected);
			}
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
		
		ImGui::End();
	}
}

void UI_Hierarchy::hierarchyRecursive(GameObject* node, GameObject* selected)
{
	for (uint i = 0; i < node->childrens.size(); ++i)
	{
		ImGuiTreeNodeFlags nodeFlags = 0;
		if (node->childrens[i] == selected)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		//if (node->childrens[i]->childrens.size() > 0)
		//{
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
			nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		//}
		//else
			//nodeFlags |= ImGuiTreeNodeFlags_Bullet;

		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)node->childrens[i]->getGOId(), nodeFlags, "%s", node->childrens[i]->getName());

		if (nodeOpen && node->childrens[i]->childrens.size() > 0)
		{
			if (node->childrens[i])
				hierarchyRecursive(node->childrens[i]);
		}
		if (ImGui::IsItemClicked())
			app->manager->select(node->childrens[i]);
	}
}