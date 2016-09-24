#include "UI_Conf.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleInput.h"

UI_Conf::UI_Conf() : UI_Comp()
{
}


UI_Conf::~UI_Conf()
{
	delete[] fps;
	delete[] ms;
}

void UI_Conf::draw()
{
	if (ImGui::Begin("Configuration"), &active)
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			static char appName[60];
			strcpy_s(appName, 60, app->getTitle());
			if (ImGui::InputText("App Name", appName, 60)) app->setTitle(appName);

			static char org[60];
			strcpy_s(org, 60, app->getOrganitzation());
			if (ImGui::InputText("Organitzation", org, 60)) app->setOrganitzation(org);

			static int maxFPS = app->getMaxFPS();
			if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 240)) app->setMaxFPS(maxFPS);				

			ImGui::Text("Limit Framrerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", maxFPS);//app->getMaxFPS());

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps[arraySize - 1]);
			ImGui::PlotHistogram("##framerate", &fps[0], arraySize, 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms[arraySize - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms[0], arraySize, 0, title, 0.0f, 40.0f, ImVec2(310, 100));

		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Text("Icon:");//TODO: add functionality to change app icon
			float brightness = app->window->getBrightness();
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) app->window->setBrightness(brightness);
		
			int width, height, minW, minH, maxW, maxH;
			app->window->getWindowSize(width, height);
			app->window->getRange(minW, minH, maxW, maxH);
			if (ImGui::SliderInt("Width", &width, minW, maxW)) app->window->setWidth(width);
			if (ImGui::SliderInt("Height", &height, minH, maxH)) app->window->setHeight(height);

			int x = 0, y = 0;
			int maxX = maxW - width;
			int maxY = maxH - height;
			app->window->getPosition(x, y);
			if (ImGui::SliderInt("Position X", &x, 0, maxX)) app->window->setPosition(x, y);
			if (ImGui::SliderInt("Position Y", &y, 0, maxY)) app->window->setPosition(x, y);

			if (ImGui::InputInt("PositionX", &x)) app->window->setPosition(x, y);
			if (ImGui::InputInt("PositionY", &y)) app->window->setPosition(x, y);

			ImGui::Text("Refresh rate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", app->window->getRefresh());

			ImGui::Separator();

			bool fullscreen = app->window->isFullScreen();
			bool resizable = app->window->isResizable();
			bool borderless = app->window->isBorderless();
			bool fullscreenDesktop = app->window->isFullscreenDesktop();
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) app->window->setFullScreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable)) app->window->setResizable(resizable);
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Restart to apply");
			if (ImGui::Checkbox("Borderless", &borderless)) app->window->setBorderless(borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("FullDesktop", &fullscreenDesktop)) app->window->setFullScreenDesktop(fullscreenDesktop);
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			bool vSync = app->renderer3D->getVSync();
			if (ImGui::Checkbox("VSync", &vSync)) app->renderer3D->setVSync(vSync);
		}

		if (ImGui::CollapsingHeader("FileSystem"))
		{
			ImGui::Text("Base path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), app->fs->getBasePath());
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			int x, y, motX, motY, w;
			x = app->input->getMouseX();
			y = app->input->getMouseY();
			motX = app->input->getMouseXMotion();
			motY = app->input->getMouseYMotion();
			w = app->input->getWheelYMotion();
			ImGui::Text("Mouse position: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d,%d", x, y);
			ImGui::Text("Mouse motion: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d,%d", motX, motY);
			ImGui::Text("Mouse wheel: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", w);
		}

		ImGui::End();
	}

	
}

void UI_Conf::pushFpsMs(float _fps, float _ms)
{
	if (arraySize == MAX_FPS_LOG)
	{
		for (uint i = 0; i < MAX_FPS_LOG - 1; ++i)
		{
			fps[i] = fps[i + 1];
			ms[i] = ms[i + 1];
		}
	}
	else
		++arraySize;

	fps[arraySize - 1] = _fps;
	ms[arraySize - 1] = _ms;
}
