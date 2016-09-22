#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "Primitive.h"

#include "ImGui/imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}


ModuleEditor::~ModuleEditor()
{
}


bool ModuleEditor::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());
	return true;
}

bool ModuleEditor::Start()
{
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	return ret;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit")) App->quit = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Examples"))
		{
			if (ImGui::MenuItem("Show")) showImGuiDemo = true;
			if (ImGui::MenuItem("Hide")) showImGuiDemo = false;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (showImGuiDemo)
	{
		ImGui::ShowTestWindow(&showImGuiDemo);
	}

	if (showGrid)
	{
		//Draw floor grid and world axis
		P_Plane floor(0, 1, 0, 0);
		floor.axis = true;
		floor.color.Set(255, 255, 255);
		floor.Render();
	}

	ImGui::Render();

	return ret;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}