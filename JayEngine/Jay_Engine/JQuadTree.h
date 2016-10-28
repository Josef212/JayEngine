/** Quadtree with AABB boxes and gameobjects, created to use on the game engine: JayEngine. */
//https://github.com/Josef212/Containers

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

	void collectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum);

	void divideNode();
	void ajustNode();
	bool intersectsAllChilds(const AABB& _box);

public:
	AABB box;
	std::list<GameObject*> objects;
	treeNode* parent = nullptr;
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

	void collectCandidates(std::vector<GameObject*>& vec, const Frustum& primitive);

public:
	treeNode* rootNode = nullptr;
};


#endif // !__JQUADTREE__