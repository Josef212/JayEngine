#ifndef __UI_HIERARCHY_H__
#define __UI_HIERARCHY_H__

#include "UI_Comp.h"
class UI_Hierarchy : public UI_Comp
{
public:
	UI_Hierarchy();
	~UI_Hierarchy();

	void draw();
};

#endif // !__UI_HIERARCHY_H__