#include "UI_Conf.h"



UI_Conf::UI_Conf() : UI_Comp()
{
}


UI_Conf::~UI_Conf()
{
}

void UI_Conf::draw()
{
	if (ImGui::Begin("Config:", &active))
	{
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("About"))
	{
		ImGui::Text("JayEngine.");
		ImGui::Text("Created by Josef21296.");
	}
}