#include "Application.h"
#include "UI_Resources.h"

#include "GameObject.h"

#include "ModuleResourceManager.h"
#include "ModuleGOManager.h"

//Importers
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterScene.h"
#include "ImporterShader.h"

//Resources
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"
#include "ResourceShader.h"

//Components
#include "Mesh.h"
#include "Material.h"

#include "OpenGL.h"


UI_Resources::UI_Resources()
{
	active = true;
	infoW = 300;
	infoH = 150;
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

		if (ImGui::CollapsingHeader("Shaders"))
		{
			if (ImGui::Button("Create new shader"))
			{
				ResourceShader* sh = (ResourceShader*)app->resourceManager->createNewResource(ResourceType::RESOURCE_SHADER);
				sh->applydefaultShader();
				app->resourceManager->shaderImporter->saveShader(sh);
			}

			resources.clear();
			app->resourceManager->getResourcesOfType(resources, ResourceType::RESOURCE_SHADER);

			shaders(resources);
		}

		//------------------------------------
		//Materials

		//------------------------------------
		//TODO:

		//------------------------------------

		ImGui::End();
	}

	if (shaderEditorOpen && currentShaderEditing)
		shaderEditor(currentShaderEditing);
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
						ImGui::Text("Exported file:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s.", res->getExportedFile());

						ImGui::Text("Instances in memory:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->countReferences());
						ImGui::TextColored(ImVec4(1, 0, 0, 1), "This might not be correct, will fix in future updates.");

						//---------------
						//Scene info???
						//---------------

						if(ImGui::Button("Load prefab to scene"))
							app->resourceManager->loadResource(res);

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

						//-------------

						if (res->isInMemory())
						{
							ImGui::Text("Num vertices:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->numVertices);

							ImGui::Text("Num indices:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->numIndices);

							ImGui::Text("Num normals:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->numNormals);

							ImGui::Text("Num tex coords:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->numTexCoords);

							ImGui::Separator();
						}

						//-------------

						if (ImGui::Button("Attach mesh to..."))
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
										static char mName[32];
										sprintf_s(mName, 32, "%s", cmp[j]->getName());
										if (ImGui::MenuItem(mName))
										{
											cmp[j]->setResource(res->getUID());
										}
									}
								}

								if (ImGui::MenuItem("Add component mesh"))
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
						ImGui::Text("Exported file:");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s.", res->getExportedFile());

						ImGui::Text("Instances in memory: ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->countReferences());
						
						if (res->isInMemory())
						{
							ImGui::Text("Width:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->width);

							ImGui::Text("Height:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->height);

							ImGui::Text("Depth:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->depth);

							ImGui::Text("Bpp:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->bpp);

							ImGui::Text("Mips:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->mips);

							ImGui::Text("Bytes:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.", res->bytes);

							ImGui::Text("Format:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s.", res->getFormatStr());

							uint texIndex = res->textureGlID;
							glBindTexture(GL_TEXTURE_2D, texIndex);
							ImTextureID texID = (void*)texIndex;
							ImVec2 texSize(64, 64);
							ImGui::Image(texID, ImVec2(texSize.x, texSize.y), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255));
							
							//if (ImGui::IsItemHovered())
							//{
							//	ImGui::BeginTooltip();
							//
							//	ImVec2 texScreenPos = ImGui::GetCursorScreenPos();
							//	float focusSZ = 32.0f;
							//	float focusX = ImGui::GetMousePos().x - texScreenPos.x - focusSZ * 0.5f;
							//	if (focusX < 0.0f) focusX = 0.0f;
							//	else if (focusX > texSize.x - focusSZ) focusX = texSize.x - focusSZ;
							//	float focusY = ImGui::GetMousePos().y - texScreenPos.y - focusSZ * 0.5f;
							//	if (focusY < 0.0f) focusY = 0.0f; 
							//	else if (focusY > texSize.y - focusSZ) focusY = texSize.y - focusSZ;
							//
							//	ImVec2 uv0 = ImVec2((focusX) / texSize.x, (focusY) / texSize.y);
							//	ImVec2 uv1 = ImVec2((focusX + focusSZ) / texSize.x, (focusY + focusSZ) / texScreenPos.y);
							//
							//	ImGui::Image(texID, ImVec2(256, 256), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 255));
							//
							//	ImGui::EndTooltip();
							//}
							//TODO: Fix this 

							glBindTexture(GL_TEXTURE_2D, 0);

							ImGui::Separator();
						}

						//-------------

						if (ImGui::Button("Attach tex to..."))
							ImGui::OpenPopup("tex popup");

						//TODO: Delete button that remove resource of the map???

						if (ImGui::BeginPopup("tex popup"))
						{
							ImGui::MenuItem("GO selected", NULL, false, false);
							ImGui::Separator();

							GameObject* selected = app->goManager->getSelected();

							if (!selected)
								ImGui::MenuItem("Not game object", NULL, false, false);
							else
							{
								//TODO: Adapt this to material component and multitextures.
								std::vector<Component*> cmp = selected->findComponent(ComponentType::MATERIAL);
								for (uint j = 0; j < cmp.size(); ++j)
								{
									if (cmp[j])
									{
										static char tName[32];
										sprintf_s(tName, 32, "%s", cmp[j]->getName());
										if (ImGui::MenuItem(tName))
										{
											cmp[j]->setResource(res->getUID());
										}
									}
								}
								
								if (ImGui::MenuItem("Add component material"))
								{
									Material* mat = (Material*)selected->addComponent(ComponentType::MATERIAL);
									if (mat)
									{
										mat->setResource(res->getUID());
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
					txSelected = i;

				ImGui::TreePop();
			}
		}
	}
}

//-----------------------------

void UI_Resources::shaders(std::vector<Resource*> shds)
{
	static int sSelected = -1;

	for (uint i = 0; i < shds.size(); ++i)
	{
		ResourceShader* res = (ResourceShader*)shds[i];
		if (res)
		{
			ImGuiTreeNodeFlags nodeFlags = 0;
			if (sSelected == i)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
				nodeFlags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			}
			else
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;

			if (ImGui::TreeNodeEx(res->shaderName.c_str(), nodeFlags))
			{
				if (sSelected == i)
				{
					if (ImGui::Button("Force compile"))
						app->resourceManager->shaderImporter->compileShader(res);

					ImGui::SameLine();

					if (ImGui::Button("Attach shader to"))
						ImGui::OpenPopup("shaders popup");

					if (ImGui::BeginPopup("shaders popup"))
					{
						ImGui::MenuItem("GO selected", NULL, false, false);
						ImGui::Separator();

						GameObject* selected = app->goManager->getSelected();

						if (!selected)
							ImGui::MenuItem("Not game object", NULL, false, false);
						else
						{
							//TODO: Adapt this to material component and multitextures.
							std::vector<Component*> cmp = selected->findComponent(ComponentType::MATERIAL);
							for (uint j = 0; j < cmp.size(); ++j)
							{
								Material* mat = (Material*)cmp[j];
								if (mat)
								{
									static char tName[32];
									sprintf_s(tName, 32, "%s", mat->getName());
									if (ImGui::MenuItem(tName))
									{
										mat->shaderResource = res;
									}
								}
							}

							if (ImGui::MenuItem("Add component material"))
							{
								Material* mat = (Material*)selected->addComponent(ComponentType::MATERIAL);
								if (mat)
								{
									mat->shaderResource = res;
								}
							}
						}


						ImGui::EndPopup();
					}


					//-----------------------------------------------
					if (ImGui::Button("Edit shader"))
					{
						//Load it to memory.
						shaderEditorOpen = true;
						currentShaderEditing = res;
						if (!res->vertexAndFragtalInMemory())
							app->resourceManager->shaderImporter->loadShaderToMemory(res);
					}
				}

				if (ImGui::IsItemClicked())
					sSelected = i;

				ImGui::TreePop();
			}
		}
	}
}

//-----------------------------

void UI_Resources::shaderEditor(ResourceShader* resShader)
{
	if (!resShader)
		return;

	ImGui::SetNextWindowSize(ImVec2(850, 760));

	ImGui::Begin("Shader editor.", &shaderEditorOpen);
	{
		char name[128];
		sprintf_s(name, 128, resShader->shaderName.c_str());
		if (ImGui::InputText("Name", name, 128)) resShader->shaderName = name;

		ImGui::Text("Shader buffer id:", resShader->shaderID);

		ImGui::SameLine();
		int s = strlen(resShader->vertexShaderStr.c_str());
		ImGui::Text("Text size:%d.  Text size pls:%d.", s, s + 50);

		ImGui::BeginChild("E", ImVec2(830, 650));
		{
			const char* text = resShader->vertexShaderStr.c_str();
			ImGui::InputTextMultiline("##shader", (char*)text, strlen(text) + 50, ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput);
			resShader->vertexShaderStr = text;

			ImGui::EndChild();
		}

		if (ImGui::Button("Compile shader"))
		{
			app->resourceManager->shaderImporter->saveShader(resShader);
			app->resourceManager->shaderImporter->compileShader(resShader);
		}

		ImGui::SameLine();

		if (ImGui::Button("Save shader"))
			app->resourceManager->shaderImporter->saveShader(resShader);

		if (ImGui::Button("Close"))
		{
			currentShaderEditing = NULL;
			shaderEditorOpen = false;
		}

		ImGui::End();
	}
}