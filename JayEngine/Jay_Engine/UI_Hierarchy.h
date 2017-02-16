#ifndef __UI_HIERARCHY__
#define __UI_HIERARCHY__

#include "UI_Panel.h"

class GameObject;

class UI_Hierarchy : public UI_Panel
{
public:
	UI_Hierarchy(bool startEnalbed = false);
	~UI_Hierarchy();

	void Draw()override;
	void HierarchyRecursive(GameObject* node, GameObject* selected = nullptr);
};

#endif // !__UI_HIERARCHY__