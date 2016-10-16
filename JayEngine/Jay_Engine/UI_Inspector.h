#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UI_Comp.h"

class GameObject;

class UI_Inspector : public UI_Comp
{
public:
	UI_Inspector();
	~UI_Inspector();

	void draw();

private:
	void drawTransformation(GameObject* selected);
	void drawMesh(GameObject* selected);
	void drawMaterial(GameObject* selected);

public:

private:
	uint windowW = 1, windowH = 1;
};

#endif // !__UI_INSPECTOR_H__