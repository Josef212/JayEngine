#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "Primitive.h"

#include "ImGui/imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	LOG("Editor: Creation.");
}


ModuleEditor::~ModuleEditor()
{
	LOG("Editor: Destroying.");
}


bool ModuleEditor::init()
{
	LOG("Editor: Init.");
	ImGui_ImplSdlGL3_Init(app->window->getWindow());
	return true;
}

bool ModuleEditor::start()
{
	LOG("Editor: Start.");
	return true;
}

update_status ModuleEditor::preUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	ImGui_ImplSdlGL3_NewFrame(app->window->getWindow());

	return ret;
}

update_status ModuleEditor::update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit")) app->quit = true;
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

bool ModuleEditor::cleanUp()
{
	LOG("Editor: CleanUp.");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditor::drawEditor()
{
	ImGui::Render();
}

void ModuleEditor::passInput(SDL_Event* inputEvent)
{
	ImGui_ImplSdlGL3_ProcessEvent(inputEvent);
}