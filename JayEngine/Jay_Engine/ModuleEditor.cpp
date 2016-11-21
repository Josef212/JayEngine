#include "Application.h"
#include "Globals.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleGOManager.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "FileParser.h"

#include "UI_Comp.h"
#include "UI_Conf.h"
#include "UI_Console.h"
#include "UI_Hierarchy.h"
#include "UI_Inspector.h"
#include "UI_Tree.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

#include "GameObject.h"

ModuleEditor::ModuleEditor(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Editor: Creation.");

	name.assign("module_editor");

	conf = new UI_Conf();
	console = new UI_Console();
	hieracy = new UI_Hierarchy();
	inspector = new UI_Inspector();
	tree = new UI_Tree();


	uiList.push_back(conf);
	uiList.push_back(console);
	uiList.push_back(hieracy);
	uiList.push_back(inspector);
	uiList.push_back(tree);
}


ModuleEditor::~ModuleEditor()
{
	_LOG(LOG_STD, "Editor: Destroying.");
}


bool ModuleEditor::init(FileParser* conf)
{
	_LOG(LOG_STD, "Editor: Init.");
	ImGui_ImplSdlGL3_Init(app->window->getWindow());

	engineVersion.assign(conf->getString("version", "0.1.0-V"));
	
	setStyle(); //TODO: config??

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
			if (ImGui::MenuItem("Load")) app->loadGame(); //TODO: open file browser etc
			if (ImGui::MenuItem("Save")) showSaveWin = !showSaveWin;
			if (ImGui::MenuItem("Serch files")) showDirWin = !showDirWin;
			if (ImGui::MenuItem("Quit")) app->quit = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration")) conf->swapActive();
			if (ImGui::MenuItem("Console")) console->swapActive();
			if (ImGui::MenuItem("Hierarchy")) hieracy->swapActive();
			if (ImGui::MenuItem("Inspector")) inspector->swapActive();
			if (ImGui::MenuItem("Scene tree")) tree->swapActive();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty Game object")) app->goManager->createEmptyGO();
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube")) app->goManager->loadCube(); //DEL_COM: commented for delivery

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Camera")) app->goManager->createCamera();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Components"))
		{
			if (ImGui::MenuItem("Add transform"))app->goManager->addTransform();
			if (ImGui::MenuItem("Add mesh"))app->goManager->addMesh();
			if (ImGui::MenuItem("Add material"))app->goManager->addMaterial();
			if (ImGui::MenuItem("Add camera"))app->goManager->addCamera();
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
				if (ImGui::MenuItem("Info")) _LOG(LOG_INFO, "Test log.");
				if (ImGui::MenuItem("Info remarkable")) _LOG(LOG_INFO_REM, "Test log.");

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("About")) showAbout = !showAbout;
			ImGui::EndMenu();
		}

		//---------------------
		//TMP
		if (ImGui::BeginMenu("FBX"))
		{
			if (ImGui::MenuItem("Load cube.fbx")) app->goManager->loadFBX("Cube.fbx", NULL);//DEL_COM
			if (ImGui::MenuItem("Load mecha.fbx")) app->goManager->loadFBX("MechaT.fbx", NULL);
			if (ImGui::MenuItem("Load brute.fbx")) app->goManager->loadFBX("Brute.fbx", NULL);
			if (ImGui::MenuItem("Load town.fbx")) app->goManager->loadFBX("Street environment_V01.FBX", NULL);
			ImGui::EndMenu();
		}
		//---------------------

		ImGui::EndMainMenuBar();
	}

	if (showAbout)
	{
		ImGui::Begin("About", &showAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("This is a 3D game engine programmed for educational purposes as a university project.");
		ImGui::Text("JayEngine is created by Josef21296 and I'm studying at UPC-CITM and it's currently on %s version.", engineVersion.c_str());
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

	if (showDirWin)
		openDirWin("Assets/fbx");


	if (showTimeDisplay)
		timeDisplay();

	playMenu();

	return ret;
}

bool ModuleEditor::cleanUp()
{
	_LOG(LOG_INFO, "Editor: CleanUp.");

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

void ModuleEditor::openDirWin(const char* path)
{
	std::vector<std::string> files;
	app->fs->getFilesOnDir(path, files);
	if (ImGui::Begin("Dir", &showDirWin))
	{
		static int selected = -1;
		ImGui::BeginChild("Files", ImVec2(200, 0), true);
		{
			for (int i = 0; i < files.size(); ++i)
			{
				if (ImGui::Selectable(files[i].c_str(), selected == i))
					selected = i;
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		if (selected > -1 && selected < files.size())
		{
			if (ImGui::Button("Load fbx"))
			{
				char file[64];
				strcpy_s(file, 64, files[selected].c_str());
				app->goManager->loadFBX(file, NULL);
				showDirWin = !showDirWin;
			}
		}
	}
	ImGui::End();
}

void ModuleEditor::setStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[0] = ImVec4(1, 1, 1, 1); //Text
	style.Colors[1] = ImVec4(1, 1, 1, 1); //Text
	style.Colors[2] = ImVec4(0.156f, 0.172f, 0.211f, 1); //Win bg
	style.Colors[3] = ImVec4(0, 0, 0, 0.5f); //Child

	style.Colors[4] = ImVec4(0.117f, 0.494f, 0.478f, 1); //Menu items


	style.Colors[10] = ImVec4(0.254f, 0.282f, 0.341f, 1); //Title
	style.Colors[11] = ImVec4(0.254f, 0.282f, 0.341f, 0.5f); //Title
	style.Colors[12] = ImVec4(0.254f, 0.282f, 0.341f, 1); //Title
	style.Colors[13] = ImVec4(0.360f, 0.396f, 0.478f, 1); //Menu bg

	style.Colors[14] = ImVec4(0.117f, 0.494f, 0.478f, 1); //bg
	style.Colors[15] = ImVec4(0.007f, 0.784f, 0.835f, 1); //thumb grab
	style.Colors[16] = ImVec4(0.015f, 0.686f, 0.749f, 1); //thumb h
	style.Colors[17] = ImVec4(0.015f, 0.686f, 0.749f, 1); //thumb ac

	style.Colors[22] = ImVec4(0.294f, 0.356f, 0.364f, 1); //Button
	style.Colors[23] = ImVec4(0.792f, 0.898f, 0.984f, 1); //Button
	style.Colors[24] = ImVec4(0.411f, 0.509f, 0.537f, 1); //Button

	/*
	style.Colors[0] -- Text color
	style.Colors[1] -- Text disabled
	style.Colors[2] -- Window background (Not the title bar)
	style.Colors[3] -- Child window background
	style.Colors[4] -- Popup background //Menu items
	style.Colors[5] -- Border
	style.Colors[6] -- Border shadow
	style.Colors[7] -- Frame background //Checkbox, text box...
	style.Colors[8] -- Frame background hovered
	style.Colors[9] -- Frame background active
	style.Colors[10] -- Title background
	style.Colors[11] -- Title background collapsed
	style.Colors[12] -- Title background active
	style.Colors[13] -- Menu bar background
	style.Colors[14] -- Scroll bar background
	style.Colors[15] -- Scroll thumb grab
	style.Colors[16] -- Scroll thumb hovered
	style.Colors[17] -- Scroll thumb grab active
	style.Colors[18] -- Combo background
	style.Colors[19] -- Check mark
	style.Colors[20] -- Slider grab
	style.Colors[21] -- Slider grab active
	style.Colors[22] -- Button
	style.Colors[23] -- Button hovered
	style.Colors[24] -- Button active
	style.Colors[25] -- Header //For the inspector tree
	style.Colors[26] -- Header hovered
	style.Colors[27] -- Header active
	style.Colors[28] -- Column ***
	style.Colors[29] -- Column hovered
	style.Colors[30] -- Column active
	style.Colors[31] -- Resize grip
	style.Colors[32] -- Resize grip hovered
	style.Colors[33] -- Resize grip active
	style.Colors[34] -- Close button
	style.Colors[35] -- Close button hovered
	style.Colors[36] -- Close button active
	style.Colors[37] -- Plot lines
	style.Colors[38] -- Plot lines hovered
	style.Colors[39] -- Plot histogram
	style.Colors[40] -- Plot histogram hovered
	style.Colors[41] -- Text selected background
	style.Colors[42] -- Modal window darkening	
	*/
}

void ModuleEditor::playMenu()
{
	static bool b = true;
	int w = ImGui::GetIO().DisplaySize.x;
	int winSize = 300;
	ImGui::SetNextWindowPos(ImVec2((w / 2) - (winSize / 2), 20));
	ImGui::SetNextWindowSize(ImVec2(winSize, 40));
	if (!ImGui::Begin("Example: Fixed Overlay", &b, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::End();
		return;
	}

	if (!app->isPlaySate() && !app->isPauseState())
	{
		if (ImGui::Button("Play")) app->setPlay();
	}
	else
	{
		if(app->isPauseState())
			if (ImGui::Button("Play")) app->setPlay();
		
		if(app->isPlaySate())
			if (ImGui::Button("Pause")) app->setPause();

		ImGui::SameLine();
		if (ImGui::Button("Stop"))app->setStop();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &app->renderer3D->showGrid);
	ImGui::SameLine();
	ImGui::Checkbox("Draw Debug", &app->debug);

	ImGui::End();
}

void ModuleEditor::openSaveBrowser(const char* path)
{
	std::vector<std::string> files;
	app->fs->getFilesOnDir(path, files);
	if (ImGui::Begin("Save", &showDirWin))
	{
		static int selected = -1;
		ImGui::BeginChild("Directory:", ImVec2(200, 0), true);
		{
			for (int i = 0; i < files.size(); ++i)
			{
				if (ImGui::Selectable(files[i].c_str(), selected == i))
					selected = i;
			}
		}
		ImGui::EndChild();

		char fileName[64];
		ImGui::InputText("", fileName, 64);
		ImGui::SameLine();

		if (selected > -1 && selected < files.size())
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "This will overwrite current file!");
			strcpy_s(fileName, 64, files[selected].c_str());
		}
		else
		{

		}
		if (ImGui::Button("Save"))
		{
			if (fileName[0] == '\0')
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "MUST add a name!");
			}
			else
			{//TODO: input will be clean??
			//app->goManager->loadFBX(file, NULL); //TODO: add goManager save scene
				showSaveWin = !showSaveWin;
			}
		}


	}
	ImGui::End();
}

void ModuleEditor::openLoadBrowser(const char* path)
{

}

void ModuleEditor::timeDisplay()
{
	ImGui::Begin("Time", &showTimeDisplay);
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Real:");
		ImGui::Separator();

		ImGui::Text("Real dt: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f", time->editorDT());

		ImGui::Text("Frames: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", time->editorFrames());
		

		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Game:");
		ImGui::Separator();

		ImGui::Text("Game dt: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f", time->DT());

		ImGui::Text("Game frames: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", time->gameFrames());

		ImGui::Separator();

		ImGui::Text("Game state:");
		ImGui::SameLine();
		switch (app->getGameState())
		{
			case gameState::PLAY:
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "PLAY");
			break;

			case gameState::PAUSE:
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "PAUSE");
			break;

			case gameState::EDITOR:
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "EDITOR");
			break;

			default:
				ImGui::TextColored(ImVec4(1, 1, 0, 1), "ERROR");
			break;
		}

		ImGui::End();
	}
}