#ifndef __UI_INSPECTOR_H__
#define __UI_INSPECTOR_H__

#include "UI_Comp.h"

class GameObject;
class Transform;
class Mesh;
class Material;
class Camera;

class UI_Inspector : public UI_Comp
{
public:
	UI_Inspector();
	~UI_Inspector();

	void draw();

private:
	void drawTransformation(GameObject* selected);
	void drawMesh(GameObject* selected, Mesh* mesh);
	void drawMaterial(GameObject* selected, Material* mat);
	void drawCamera(GameObject* selected, Camera* cam);

public:

private:
	uint windowW = 1, windowH = 1;
};

#endif // !__UI_INSPECTOR_H__