#include "Application.h"
#include "UI_Resources.h"

#include "GameObject.h"

#include "ModuleResourceManager.h"
#include "ModuleGOManager.h"

#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterScene.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"

#include <vector>


UI_Resources::UI_Resources()
{
	active = true;
}


UI_Resources::~UI_Resources()
{
}

void UI_Resources::draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 580));
	ImGui::SetNextWindowSize(ImVec2(360, 440));

	ImGui::Begin("Resources", &active);
	{
		std::vector<Resource*> resources;

		//------------------------------------
		//Prefabs

		if (ImGui::CollapsingHeader("Prefabs"))
		{
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_SCENE);

			static int scSelected = -1;

			for (uint i = 0; i < resources.size(); ++i)
			{
				ResourceScene* scRes = (ResourceScene*)resources[i];
				if (scRes)
				{
					ImGuiTreeNodeFlags nodeFlags = 0;
					if (scSelected == i)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
					}
					else
						nodeFlags |= ImGuiTreeNodeFlags_Leaf;
					
					if (ImGui::TreeNodeEx(scRes->getOriginalFile(), nodeFlags))
					{
						if (ImGui::IsItemClicked())
							scSelected = i;

						ImGui::TreePop();
					}
				}
			}
		}

		//------------------------------------
		//Textures

		if (ImGui::CollapsingHeader("Textures"))
		{
			resources.clear();
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_TEXTURE);

			static int txSelected = -1;

			for (uint i= 0; i < resources.size(); ++i)
			{
				ResourceTexture* scRes = (ResourceTexture*)resources[i];
				if (scRes)
				{
					ImGuiTreeNodeFlags nodeFlags = 0;
					if (txSelected == i)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
					}
					else
						nodeFlags |= ImGuiTreeNodeFlags_Leaf;

					if (ImGui::TreeNodeEx(scRes->getOriginalFile(), nodeFlags))
					{
						if (ImGui::IsItemClicked())
							txSelected = i;

						ImGui::TreePop();
					}
				}
			}
		}

		//------------------------------------
		//Meshes

		if (ImGui::CollapsingHeader("Meshes"))
		{
			resources.clear();
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_MESH);

			static int mSelected = -1;

			for (uint i = 0; i < resources.size(); ++i)
			{
				ResourceMesh* scRes = (ResourceMesh*)resources[i];
				if (scRes)
				{
					ImGuiTreeNodeFlags nodeFlags = 0;
					if (mSelected == i)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
						nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
					}
					else
						nodeFlags |= ImGuiTreeNodeFlags_Leaf;

					if (ImGui::TreeNodeEx(scRes->getExportedFile(), nodeFlags))
					{
						if (ImGui::IsItemClicked())
							mSelected = i;

						ImGui::TreePop();
					}
				}
			}
		}

		//------------------------------------
		//Shaders

		//------------------------------------
		//Materials

		//------------------------------------
		//TODO:

		//------------------------------------

		ImGui::End();
	}
}