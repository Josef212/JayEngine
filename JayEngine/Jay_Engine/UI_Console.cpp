#include "UI_Console.h"
#include "Globals.h"

UI_Console::UI_Console() : UI_Comp()
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

		static char input[100];
		static bool focus = true;
		if (ImGui::InputText("", input, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (focus)
				ImGui::SetKeyboardFocusHere();
			//TODO: add console and its functionality
			LOG(input);
			*input = '\0';
		}
		
		if (scrollDown)
		{
			scrollDown = false;
			ImGui::SetScrollHere(1.0f);
		}

		ImGui::End();
	}
}

void UI_Console::logUi(const char* str)
{
	logs.append(str);
	scrollDown = true;
}