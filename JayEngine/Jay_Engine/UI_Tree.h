#ifndef __UITREE_H__
#define __UITREE_H__

#include "UI_Comp.h"

class oTreeNode;

class UI_Tree : public UI_Comp
{
public:
	UI_Tree();
	virtual ~UI_Tree();

	void draw();

private:
	void recursiveDrawTree(oTreeNode* treeNode, uint index);
};

#endif // !__UITREE_H__