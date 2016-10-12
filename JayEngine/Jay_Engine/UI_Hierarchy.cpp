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

		ImGui::End();
	}
}