#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "UI_Comp.h"
#include "UI_Conf.h"
#include "UI_Console.h"

#include "ImGui/imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(bool startEnabled) : Module(startEnabled)
{
	LOG("Editor: Creation.");
	conf = new UI_Conf();
	console = new UI_Console();

	uiList.push_back(conf);
	uiList.push_back(console);
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

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console")) console->swapActive();
			if (ImGui::MenuItem("Configuration")) conf->swapActive();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo")) showImGuiDemo = !showImGuiDemo;
			if (ImGui::MenuItem("Documentation")) app->browse(WIKI_URL);
			if (ImGui::MenuItem("Check all releases")) app->browse(RELEASES_URL);
			if (ImGui::MenuItem("Report a bug")) app->browse(ISSUES_URL);
			if (ImGui::MenuItem("About")) showAbout = !showAbout;
			ImGui::EndMenu();
		}
		

		ImGui::EndMainMenuBar();
	}

	if (showAbout)
	{
		ImGui::Begin("About", &showAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("This is a 3D game engine programmed for educational purposes as a university project.");
		ImGui::Text("JayEngine is created by Josef21296 and I'm at UPC-CITM and it's currently at %s version.", ENGINE_VERSION);
		ImGui::Text("Please report any bug you find, take a look at 'Help->Report a bug'. Thanks ;)");
		ImGui::Text("Check the repository clicking here:");
		ImGui::SameLine();
		if (ImGui::Button("GitHub")) app->browse(REPOSITORY_URL);
		ImGui::SameLine();
		ImGui::Text("or know more about me clicking here:");
		ImGui::SameLine();
		if (ImGui::Button("Josef21296")) app->browse(GITHUB_URL);

		ImGui::End();
	}

	std::list<UI_Comp*>::iterator it = uiList.begin();
	for (; it != uiList.end(); ++it)
	{
		if ((*it)->isActive())
			(*it)->draw();
	}

	if (showImGuiDemo)
	{
		ImGui::ShowTestWindow(&showImGuiDemo);
		ImGui::ShowMetricsWindow(&showImGuiDemo);
	}

	return ret;
}

bool ModuleEditor::cleanUp()
{
	LOG("Editor: CleanUp.");

	std::list<UI_Comp*>::iterator it = uiList.begin();
	for (; it != uiList.end(); ++it)
		RELEASE((*it));

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

void ModuleEditor::logFPS(float fps, float ms)
{
	if (conf)
		conf->pushFpsMs(fps, ms);
}

void ModuleEditor::log(const char* str)
{
	if (console)
		console->logUi(str);
}