#ifndef __MODULE_EDITOR__
#define __MODULE_EDITOR__

#include "Module.h"
#include <list>
#include <string>

class UI_Panel;
class UI_Conf;
class UI_Console;
class UI_Hierarchy;
class UI_Inspector;
class UI_Tree;
class UI_Resources;

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool startEnabled = true);
	~ModuleEditor();

	bool Init(FileParser* conf)override;
	bool Start()override;
	update_status PreUpdate(float dt)override;
	update_status Update(float dt)override;
	bool CleanUp()override;

	void DrawEditor();
	void PassInput(SDL_Event* inputEvent);

	void LogFPS(float fps, float ms);
	void Log(const char* str, logType type);

	bool UsingMouse()const;
	bool UsingKeyboard()const;

private:
	void OpenDirWin(const char* path);
	void OpenSaveBrowser(const char* path);
	void OpenLoadBrowser(const char* path);
	void PlayMenu();
	void SetStyle();
	void TimeDisplay();

public:
	std::list<UI_Panel*> uiList;

	UI_Conf* conf = nullptr;
	UI_Console* console = nullptr;
	UI_Hierarchy* hieracy = nullptr;
	UI_Inspector* inspector = nullptr;
	UI_Tree* tree = nullptr;
	UI_Resources* resources = nullptr;

private: //TODO: So many booleans are needed????
	bool showImGuiDemo = false;
	bool showConfig = false;
	bool showAbout = false;
	bool showDirWin = false;
	bool showSaveWin = false;
	bool showLoadWin = false;
	bool showTimeDisplay = true;

	std::string engineVersion ;

	bool isUsingMouse = false;
	bool isUsingKeyboard = false;

};

#endif // !__MODULE_EDITOR__