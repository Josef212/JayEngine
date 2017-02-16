#ifndef __UI_INSPECTOR__
#define __UI_INSPECTOR__

#include "UI_Panel.h"

class GameObject;
class Transform;
class Mesh;
class Material;
class Camera;

class UI_Inspector : public UI_Panel
{
public:
	UI_Inspector(bool startEnalbed = false);
	~UI_Inspector();

	void Draw()override;

private:
	void DrawTransformation(GameObject* selected);
	void DrawMesh(GameObject* selected, Mesh* mesh);
	void DrawMaterial(GameObject* selected, Material* mat);
	void DrawCamera(GameObject* selected, Camera* cam);

public:

private:
	uint windowW = 1, windowH = 1;
};

#endif // !__UI_INSPECTOR__