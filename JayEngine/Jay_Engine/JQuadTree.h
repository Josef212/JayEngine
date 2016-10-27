#ifndef __JQUADTREE__
#define __JQUADTREE__

#include "Math.h"
#include <list>
#include <vector>

class GameObject;

//---------------------------------------------------
//---------------TreeNode----------------------------
//---------------------------------------------------

class treeNode
{
public:
	treeNode(const AABB& _box);
	~treeNode();

	void insert(GameObject* obj);
	void erase(GameObject* obj);

	void coollectBoxes(std::vector<AABB>& vec);
	void coollectGO(std::vector<GameObject*>& vec);
	//TODO: intersection funct(template for primitives)


	void divideNode();
	void ajustNode();
	bool intersectsAllChilds(const AABB& _box);

public:
	AABB box;
	std::list<GameObject*> objects;
	treeNode* parent = NULL;
	treeNode* childs[4];
};

//---------------------------------------------------
//---------------JQuadTree---------------------------
//---------------------------------------------------

class JQuadTree
{
public:
	JQuadTree();
	virtual ~JQuadTree();

	void insert(GameObject* obj);
	void erase(GameObject* obj);

	void setRoot(const AABB& _box);
	void clear();

public:
	treeNode* rootNode = NULL;
};


#endif // !__JQUADTREE__