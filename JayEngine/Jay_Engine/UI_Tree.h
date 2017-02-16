#ifndef __UI_TREE__
#define __UI_TREE__

#include "UI_Panel.h"

class OcTreeNode;

class UI_Tree : public UI_Panel
{
public:
	UI_Tree(bool startEnalbed = false);
	virtual ~UI_Tree();

	void Draw()override;

private:
	void RecursiveDrawTree(OcTreeNode* treeNode, uint index);
};

#endif // !__UI_TREE__