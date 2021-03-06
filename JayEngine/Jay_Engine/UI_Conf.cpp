#include "UI_Conf.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleInput.h"
#include "HrdInfo.h"
#include "ModuleCamera3D.h"
#include "Camera.h"

#include "mmgr\mmgr.h"

UI_Conf::UI_Conf(bool startEnalbed) : UI_Panel(startEnalbed)
{
}


UI_Conf::~UI_Conf()
{}

void UI_Conf::Draw()
{
	ImGui::Begin("Configuration", &active);
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			static char appName[60];
			strcpy_s(appName, 60, app->GetTitle());
			if (ImGui::InputText("App Name", appName, 60)) app->SetTitle(appName);

			static char org[60];
			strcpy_s(org, 60, app->GetOrganitzation());
			if (ImGui::InputText("Organitzation", org, 60)) app->SetOrganitzation(org);

			static int maxFPS = app->GetMaxFPS();
			if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 240)) app->SetMaxFPS(maxFPS);				

			ImGui::Text("Limit Framrerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", maxFPS);//app->GetMaxFPS());

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps[arraySize - 1]);
			ImGui::PlotHistogram("##framerate", &fps[0], arraySize, 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms[arraySize - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms[0], arraySize, 0, title, 0.0f, 40.0f, ImVec2(310, 100));


			// Memory --------------------
			sMStats stats = m_getMemoryStatistics();
			static int speed = 0;
			static std::vector<float> memory(100);
			if (++speed > 20)
			{
				speed = 0;
				if (memory.size() == 100)
				{
					for (uint i = 0; i < 100 - 1; ++i)
						memory[i] = memory[i + 1];

					memory[100 - 1] = (float)stats.totalReportedMemory;
				}
				else
					memory.push_back((float)stats.totalReportedMemory);
			}

			ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

			ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
			ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
			ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
			ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
			ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
			ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Text("Icon:");//TODO: add functionality to change app icon
			float brightness = app->window->GetBrightness();
			if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f)) app->window->SetBrightness(brightness);
		
			int width, height, minW, minH, maxW, maxH;
			app->window->GetWindowSize(width, height);
			app->window->GetRange(minW, minH, maxW, maxH);
			if (ImGui::SliderInt("Width", &width, minW, maxW)) app->window->SetWidth(width);
			if (ImGui::SliderInt("Height", &height, minH, maxH)) app->window->SetHeight(height);

			int x = 0, y = 0;
			int maxX = maxW - width;
			int maxY = maxH - height;
			app->window->GetPosition(x, y);
			if (ImGui::SliderInt("Position X", &x, 0, maxX)) app->window->SetPosition(x, y);
			if (ImGui::SliderInt("Position Y", &y, 0, maxY)) app->window->SetPosition(x, y);

			if (ImGui::InputInt("PositionX", &x)) app->window->SetPosition(x, y);
			if (ImGui::InputInt("PositionY", &y)) app->window->SetPosition(x, y);

			ImGui::Text("Refresh rate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i", app->window->GetRefresh());

			ImGui::Separator();

			bool fullscreen = app->window->IsFullScreen();
			bool resizable = app->window->IsResizable();
			bool borderless = app->window->IsBorderless();
			bool fullscreenDesktop = app->window->IsFullscreenDesktop();
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) app->window->SetFullScreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable)) app->window->SetResizable(resizable);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.69, 0.13, 0.13, 1), "Reset app too apply."); //TODO: add functionality to save and load to change resizale
			if (ImGui::Checkbox("Borderless", &borderless)) app->window->SetBorderless(borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("FullDesktop", &fullscreenDesktop)) app->window->SetFullScreenDesktop(fullscreenDesktop);
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			bool vSync = app->renderer3D->GetVSync();
			if (ImGui::Checkbox("VSync", &vSync)) app->renderer3D->SetVSync(vSync);
		}

		if (ImGui::CollapsingHeader("EditorCamera"))
		{
			Camera* cam = app->camera->GetCamera();
			ImGui::ColorEdit4("Background:", (float*)&cam->background, false);

			float nearP = cam->GetNearPlaneDist();
			float farP = cam->GetFarPlaneDist();
			float fov = cam->GetFOV();

			if (ImGui::DragFloat("Near plane:", &nearP)) cam->SetNearPlaneDist(nearP);
			if (ImGui::DragFloat("Far plane:", &farP))cam->SetFarPlaneDist(farP);
			if (ImGui::DragFloat("Field of view:", &fov))cam->SetFOV(fov);

			ImGui::DragFloat("Move speed: ", &app->camera->moveSpeed);
			ImGui::DragFloat("Rotation speed: ", &app->camera->rotSpeed);
			ImGui::DragFloat("Zoom speed: ", &app->camera->zoomSpeed);
			ImGui::DragFloat("Orbiting distance: ", &app->camera->distToReference);

			ImGui::Text("Aspect ratio: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f.", cam->GetAspectRatio());

			bool culling = cam->IsCullingActive();
			if (ImGui::Checkbox("Culling", &culling))cam->SetCulling(culling);
		}

		if (ImGui::CollapsingHeader("FileSystem"))
		{
			ImGui::Text("Base path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), app->fs->GetBasePath());

			ImGui::Text("Read path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "./");

			ImGui::Text("Wite path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "./");
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			int x, y, motX, motY, w;
			x = app->input->GetMouseX();
			y = app->input->GetMouseY();
			motX = app->input->GetMouseXMotion();
			motY = app->input->GetMouseYMotion();
			w = app->input->GetWheelYMotion();
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

		if (ImGui::CollapsingHeader("Information"))
		{
			ImGui::Text("SDL Version: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", app->info->GetInfo()->sdlVersion.major, app->info->GetInfo()->sdlVersion.minor, app->info->GetInfo()->sdlVersion.patch);

			ImGui::Separator();

			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dCores (Cache: %dKb)", app->info->GetInfo()->cpuCores, app->info->GetInfo()->cpuCacheSize);

			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dGb", app->info->GetInfo()->ram);

			ImGui::Text("Caps: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", app->info->GetCaps());

			ImGui::Separator();

			ImGui::TextColored(ImVec4(1, 0, 0, 1), "TODO: get GPU info");
		}

		ImGui::End();
	}

	
}

void UI_Conf::PushFpsMs(float _fps, float _ms)
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
