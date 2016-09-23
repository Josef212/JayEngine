#pragma once

#include "UI_Comp.h"

class UI_Conf : public UI_Comp
{
public:
	UI_Conf();
	~UI_Conf();

	void draw();

	bool showAbout = false;
};

