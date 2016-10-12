#ifndef __UI_HIERARCHY_H__
#define __UI_HIERARCHY_H__

#include "UI_Comp.h"

class GameObject;

class UI_Hierarchy : public UI_Comp
{
public:
	UI_Hierarchy();
	~UI_Hierarchy();

	void draw();
	void hierarchyRecursive(GameObject* node, GameObject* selected = NULL);
};

#endif // !__UI_HIERARCHY_H__