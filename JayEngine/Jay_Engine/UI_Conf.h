#ifndef __UICONF_H__
#define __UICONF_H__

#include "UI_Comp.h"

#define MAX_FPS_LOG 100

class UI_Conf : public UI_Comp
{
public:
	UI_Conf(Application* app);
	~UI_Conf();

	void draw();
	void pushFpsMs(float _fps, float _ms);

public:

private:
	float fps[MAX_FPS_LOG];
	float ms[MAX_FPS_LOG];
	int arraySize = 0;
};

#endif //__UICONF_H__