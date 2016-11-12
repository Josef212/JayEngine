#include "Application.h"
#include "UI_Tree.h"
#include "Globals.h"
#include "JOctree.h"
#include "ModuleManager.h"
#include "GameObject.h"

UI_Tree::UI_Tree()
{
}


UI_Tree::~UI_Tree()
{
}

void UI_Tree::draw()
{
	ImGui::Begin("Scene tree", &active);
	{
		if(ImGui::BeginMenu("Show"))
		{
			if (ImGui::MenuItem("Show tree")) app->manager->showTree = !app->manager->showTree;
			ImGui::EndMenu();
		}

		if (app->manager->sceneTree)
		{
			if (ImGui::TreeNodeEx("Tree"))
			{
				//TODO / TMP
				//recursiveDrawTree(app->manager->sceneTree->rootNode, 0);
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void UI_Tree::recursiveDrawTree(oTreeNode* treeNode, uint index)
{
	if (treeNode)
	{
		char label[32];
		sprintf_s(label, 32, "Node %d", treeNode->objects.size());
		if (ImGui::TreeNodeEx(label))
		{
			/*ImGui::BeginChild("", ImVec2(200, 30), true);
			ImGui::Text("Objects: %d", treeNode->objects.size());
			ImGui::EndChild();*/

			for (std::list<GameObject*>::iterator it = treeNode->objects.begin(); it != treeNode->objects.end(); ++it)
			{
				ImGui::TreeNodeEx((*it)->getName(), ImGuiTreeNodeFlags_Leaf);
				if (ImGui::IsItemClicked())
					app->manager->select((*it));
				ImGui::TreePop();
			}

			for (unsigned int i = 0; i < 8; ++i)
				if (treeNode->childs[i])
					recursiveDrawTree(treeNode->childs[i], i);

			ImGui::TreePop();
		}
	}
}