#pragma once

#include "UI_Comp.h"
#include <vector>

class UI_Conf : public UI_Comp
{
public:
	UI_Conf(Application* app);
	~UI_Conf();

	void draw();
	void pushFpsMs(float _fps, float _ms);

public:
	bool showAbout = false;

private:
	std::vector<float> fps;
	std::vector<float> ms;
};

