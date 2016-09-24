#ifndef __UICONF_H__
#define __UICONF_H__

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

private:
	std::vector<float> fps;
	std::vector<float> ms;
};

#endif //__UICONF_H__