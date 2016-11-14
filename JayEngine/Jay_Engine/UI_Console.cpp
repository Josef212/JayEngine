#include "UI_Console.h"
#include "Globals.h"

UI_Console::UI_Console() : UI_Comp()
{
	lastLogColor = ImColor(255, 255, 255);
}

UI_Console::~UI_Console()
{
}

void UI_Console::draw()
{
	ImGui::Begin("Console", &active);
	{
		//ImGui::TextUnformatted(logs.begin());
		for (uint i =0; i < colors.size(); ++i)
		{
			ImGui::TextColored(colors[i], log[i].c_str());
		}

		static char input[100];
		static bool focus = true;
		if (ImGui::InputText("", input, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (focus)
				ImGui::SetKeyboardFocusHere();
			//TODO: add console and its functionality
			_LOG(LOG_CMD, input);
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

void UI_Console::logUi(const char* str, logType type)
{
	//logs.append(str);
	scrollDown = true;
	switch (type)
	{
	case LOG_STD:
		lastLogColor = ImColor(255, 255, 255);
		break;
	case LOG_ERROR:
		lastLogColor = ImColor(255, 0, 0);
		break;
	case LOG_WARN:
		lastLogColor = ImColor(255, 255, 0);
		break;
	case LOG_CMD:
		lastLogColor = ImColor(51, 153, 255);
		break;
	case LOG_INFO:
		lastLogColor = ImColor(15, 149, 0);
		break;
	case LOG_INFO_REM:
		lastLogColor = ImColor(255, 128, 0);
		break;
	default:
		lastLogColor = ImColor(255, 255, 255);
		break;
	}
	log.push_back(str);
	colors.push_back(lastLogColor);
}