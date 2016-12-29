#include "Application.h"
#include "UI_Resources.h"

#include "GameObject.h"

#include "ModuleResourceManager.h"
#include "ModuleGOManager.h"

//Importers
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterScene.h"

//Resources
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"

//Components
#include "Mesh.h"
#include "Material.h"


UI_Resources::UI_Resources()
{
	active = true;
	infoW = 300;
	infoH = 100;
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

			prefabs(resources);
		}

		//------------------------------------
		//Textures

		if (ImGui::CollapsingHeader("Textures"))
		{
			resources.clear();
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_TEXTURE);

			textures(resources);
		}

		//------------------------------------
		//Meshes

		if (ImGui::CollapsingHeader("Meshes"))
		{
			resources.clear();
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_MESH);

			meshes(resources);
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


//-----------------------------

void UI_Resources::prefabs(std::vector<Resource*> prefs)
{
	static int scSelected = -1;

	for (uint i = 0; i < prefs.size(); ++i)
	{
		ResourceScene* res = (ResourceScene*)prefs[i];
		if (res)
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

			if (ImGui::TreeNodeEx(res->getOriginalFile(), nodeFlags))
			{
				if (scSelected == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
					ImGui::BeginChild("P", ImVec2(infoW, infoH));
					{
						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->countReferences());

						//Load prefab

						ImGui::EndChild();
						ImGui::PopStyleVar();
					}
				}

				if (ImGui::IsItemClicked())
					scSelected = i;

				ImGui::TreePop();
			}
		}
	}
}

//-----------------------------

void UI_Resources::meshes(std::vector<Resource*> meshes)
{
	static int mSelected = -1;

	for (uint i = 0; i < meshes.size(); ++i)
	{
		ResourceMesh* res = (ResourceMesh*)meshes[i];
		if (res)
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

			if (ImGui::TreeNodeEx(res->getExportedFile(), nodeFlags))
			{
				if (mSelected == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
					ImGui::BeginChild("M", ImVec2(infoW, infoH));
					{
						ImGui::Text("Original file:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s.", res->getOriginalFile());

						ImGui::Text("Instances in memory:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->countReferences());

						//TODO: If resource is loaded show vertex and indices info.

						if (ImGui::Button("Attach to..."))
							ImGui::OpenPopup("meshes popup");

						//TODO: Delete button that remove resource of the map???

						if (ImGui::BeginPopup("meshes popup"))
						{
							ImGui::MenuItem("GO selected", NULL, false, false);
							ImGui::Separator();

							GameObject* selected = app->goManager->getSelected();

							if (!selected)
								ImGui::MenuItem("Not game object", NULL, false, false);
							else
							{
								std::vector<Component*> cmp = selected->findComponent(ComponentType::MESH);
								for (uint j = 0; j < cmp.size(); ++j)
								{
									if (cmp[j])
									{
										static char cmpName[32];
										sprintf_s(cmpName, 32, "%s", cmp[j]->getName());
										if (ImGui::MenuItem(cmpName))
										{
											cmp[j]->setResource(res->getUID());
										}
									}
								}

								if (ImGui::MenuItem("Add component"))
								{
									Mesh* m = (Mesh*)selected->addComponent(ComponentType::MESH);
									if (m)
									{
										m->setResource(res->getUID());
									}
								}

							}
							
							ImGui::EndPopup();
						}

						ImGui::EndChild();
						ImGui::PopStyleVar();
					}
				}

				if (ImGui::IsItemClicked())
					mSelected = i;

				ImGui::TreePop();
			}
		}
	}
}

//-----------------------------

void UI_Resources::textures(std::vector<Resource*> texs)
{
	static int txSelected = -1;

	for (uint i = 0; i < texs.size(); ++i)
	{
		ResourceTexture* res = (ResourceTexture*)texs[i];
		if (res)
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

			if (ImGui::TreeNodeEx(res->getOriginalFile(), nodeFlags))
			{
				if (txSelected == i)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
					ImGui::BeginChild("T", ImVec2(infoW, infoH));
					{
						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->countReferences());

						ImGui::EndChild();
						ImGui::PopStyleVar();
					}
				}

				if (ImGui::IsItemClicked())
					txSelected = i;

				ImGui::TreePop();
			}
		}
	}
}

//-----------------------------

void UI_Resources::shaders(std::vector<Resource*> shds)
{

}

//-----------------------------