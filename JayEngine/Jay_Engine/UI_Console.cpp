#include "UI_Console.h"



UI_Console::UI_Console(Application* app) : UI_Comp(app)
{
}

UI_Console::~UI_Console()
{
}

void UI_Console::draw()
{
	if (ImGui::Begin("Console"), &active)
	{
		ImGui::TextUnformatted(logs.begin());
		ImGui::End();
	}
}

void UI_Console::logUi(const char* str)
{
	logs.append(str);
}