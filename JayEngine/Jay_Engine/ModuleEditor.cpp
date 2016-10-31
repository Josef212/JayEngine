#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleManager.h"

#include "UI_Comp.h"
#include "UI_Conf.h"
#include "UI_Console.h"
#include "UI_Hierarchy.h"
#include "UI_Inspector.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

#include "ModuleManager.h"
#include "GameObject.h"

ModuleEditor::ModuleEditor(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Editor: Creation.");

	name.assign("module_editor");

	conf = new UI_Conf();
	console = new UI_Console();
	hieracy = new UI_Hierarchy();
	inspector = new UI_Inspector();


	uiList.push_back(conf);
	uiList.push_back(console);
	uiList.push_back(hieracy);
	uiList.push_back(inspector);
}


ModuleEditor::~ModuleEditor()
{
	_LOG(LOG_STD, "Editor: Destroying.");
}


bool ModuleEditor::init()
{
	_LOG(LOG_STD, "Editor: Init.");
	ImGui_ImplSdlGL3_Init(app->window->getWindow());
	return true;
}

bool ModuleEditor::start()
{
	_LOG(LOG_STD, "Editor: Start.");
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

	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit")) app->quit = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration")) conf->swapActive();
			if (ImGui::MenuItem("Console")) console->swapActive();
			if (ImGui::MenuItem("Hierarchy")) hieracy->swapActive();
			if (ImGui::MenuItem("Inspector")) inspector->swapActive();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty Game object")) app->manager->createEmptyGO();
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube")) app->manager->loadCube();

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Camera")) app->manager->createCamera();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Components"))
		{
			if (ImGui::MenuItem("Add transform"))app->manager->addTransform();
			if (ImGui::MenuItem("Add mesh"))app->manager->addMesh();
			if (ImGui::MenuItem("Add material"))app->manager->addMaterial();
			if (ImGui::MenuItem("Add camera"))app->manager->addCamera();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo")) showImGuiDemo = !showImGuiDemo;
			if (ImGui::MenuItem("Engine Documentation")) app->browse(WIKI_URL);
			if (ImGui::MenuItem("Check all releases")) app->browse(RELEASES_URL);
			if (ImGui::MenuItem("Report a bug")) app->browse(ISSUES_URL);
			if (ImGui::BeginMenu("3rd Party Documentation"))
			{
				if (ImGui::MenuItem("Assimp")) app->browse(ASSIMP_DOC_URL);
				if (ImGui::MenuItem("Bullet")) app->browse(BULLET_DOC_URL);
				if (ImGui::MenuItem("Devil")) app->browse(DEVIL_DOC_URL);
				if (ImGui::MenuItem("ImGui")) app->browse(IMGUI_DOC_URL);
				if (ImGui::MenuItem("MathGeolib")) app->browse(MATHGEOLIB_DOC_URL);
				if (ImGui::MenuItem("OpenGL")) app->browse(OPENGL_DOC_URL);
				if (ImGui::MenuItem("PhysFs")) app->browse(PHYSFS_DOC_URL);
				if (ImGui::MenuItem("SDL2")) app->browse(SDL_DOC_URL);
				if (ImGui::MenuItem("SDL_Mixer")) app->browse(SDL_MIXER_DOC_URL);

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Test LOG"))
			{
				if (ImGui::MenuItem("Std")) _LOG(LOG_STD, "Test log.");
				if (ImGui::MenuItem("Error")) _LOG(LOG_ERROR, "Test log.");
				if (ImGui::MenuItem("Warning")) _LOG(LOG_WARN, "Test log.");
				if (ImGui::MenuItem("Command")) _LOG(LOG_CMD, "Test log.");
				if (ImGui::MenuItem("Audio")) _LOG(LOG_AUDIO, "Test log.");
				if (ImGui::MenuItem("Camera")) _LOG(LOG_CAMERA, "Test log.");
				if (ImGui::MenuItem("Editor")) _LOG(LOG_EDITOR, "Test log.");
				if (ImGui::MenuItem("FS")) _LOG(LOG_FS, "Test log.");
				if (ImGui::MenuItem("Import")) _LOG(LOG_IMPORT, "Test log.");
				if (ImGui::MenuItem("Input")) _LOG(LOG_INPUT, "Test log.");
				if (ImGui::MenuItem("Manager")) _LOG(LOG_MANAGER, "Test log.");
				if (ImGui::MenuItem("Physics")) _LOG(LOG_PHYSICS, "Test log.");
				if (ImGui::MenuItem("Render")) _LOG(LOG_REN, "Test log.");
				if (ImGui::MenuItem("Window")) _LOG(LOG_WIN, "Test log.");

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("About")) showAbout = !showAbout;
			ImGui::EndMenu();
		}

		//---------------------
		//TMP
		if (ImGui::BeginMenu("FBX"))
		{
			if (ImGui::MenuItem("Load cube.fbx")) app->manager->loadFBX("Cube.fbx", NULL);
			if (ImGui::MenuItem("Load brute.fbx")) app->manager->loadFBX("Brute.fbx", NULL);
			if (ImGui::MenuItem("Load mecha.fbx")) app->manager->loadFBX("MechaT.fbx", NULL);
			if (ImGui::MenuItem("Load town.fbx")) app->manager->loadFBX("Street environment_V01.FBX", NULL);
			ImGui::EndMenu();
		}
		//---------------------

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
	_LOG(LOG_EDITOR, "Editor: CleanUp.");

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

void ModuleEditor::log(const char* str, logType type)
{
	if (console)
		console->logUi(str, type);
}