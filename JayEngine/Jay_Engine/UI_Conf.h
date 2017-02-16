#ifndef __UI_CONF__
#define __UI_CONF__

#include "UI_Panel.h"

#define MAX_FPS_LOG 100

class UI_Conf : public UI_Panel
{
public:
	UI_Conf(bool startEnalbed = false);
	~UI_Conf();

	void Draw()override;
	void PushFpsMs(float _fps, float _ms);

public:

private:
	float fps[MAX_FPS_LOG];
	float ms[MAX_FPS_LOG];
	int arraySize = 0;
};

#endif //__UI_CONF__