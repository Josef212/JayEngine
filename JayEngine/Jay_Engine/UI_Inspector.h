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

	void selectGameObject(GameObject* gameObj);

private:
	void drawTransformation();
	void drawMesh();
	void drawMaterial();

public:

private:
	GameObject* selectedGameObject = NULL;
};

#endif // !__UI_INSPECTOR_H__