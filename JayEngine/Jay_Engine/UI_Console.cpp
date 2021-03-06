#include "UI_Console.h"
#include "Globals.h"

UI_Console::UI_Console(bool startEnalbed) : UI_Panel(startEnalbed)
{
	lastLogColor = ImColor(255, 255, 255);
}

UI_Console::~UI_Console()
{
}

void UI_Console::Draw()
{
	int w = ImGui::GetIO().DisplaySize.x;
	int h = ImGui::GetIO().DisplaySize.y;

	//ImGui::SetNextWindowPos(ImVec2(360, 830));
	//ImGui::SetNextWindowSize(ImVec2(1560, 190));
	ImGui::SetNextWindowPos(ImVec2(w*0.2f, h*0.8f));
	ImGui::SetNextWindowSize(ImVec2(w*0.8f, h*0.2f));

	ImGui::Begin("Console", &active);
	{
		if (ImGui::Button("Clear"))
		{
			colors.clear();
			log.clear();
		}

		ImGui::BeginChild("", ImVec2(1540, 115), true);
		for (uint i =0; i < colors.size(); ++i)
		{
			ImGui::TextColored(colors[i], log[i].c_str());
		}
		ImGui::EndChild();

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

void UI_Console::LogUi(const char* str, logType type)
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