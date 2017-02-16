/** Octree with AABB boxes and gameobjects, created to use on the game engine: JayEngine. */
//https://github.com/Josef212/Containers

#ifndef __JOCTTREE__
#define __JOCTTREE__

#include "Math.h"
#include <list>
#include <vector>

class GameObject;

//---------------------------------------------------
//---------------TreeNode----------------------------
//---------------------------------------------------

class OcTreeNode
{
public:
	OcTreeNode(const AABB& _box);
	~OcTreeNode();

	void Insert(GameObject* obj);
	void Erase(GameObject* obj);

	void CoollectBoxes(std::vector<AABB>& vec);
	void CoollectGO(std::vector<GameObject*>& vec);

	void CollectTreeBoxes(std::vector<AABB>& vec);

	void DivideNode();
	void AjustNode();
	bool IntersectsAllChilds(const AABB& _box);

	void CollectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum);

public:
	AABB box;
	std::list<GameObject*> objects;
	OcTreeNode* parent = nullptr;
	OcTreeNode* childs[8];
};

//---------------------------------------------------
//---------------JQuadTree---------------------------
//---------------------------------------------------

class JOctree
{
public:
	JOctree();
	virtual ~JOctree();

	void Insert(GameObject* obj);
	void Erase(GameObject* obj);

	void SetRoot(const AABB& _box);
	void Clear();

	void CoollectBoxes(std::vector<AABB>& vec);
	void CoollectGO(std::vector<GameObject*>& vec);

	void CollectTreeBoxes(std::vector<AABB>& vec);

	void CollectCandidates(std::vector<GameObject*>& vec, const Frustum& primitive);

public:
	OcTreeNode* rootNode = nullptr;
};


#endif // !__JOCTTREE__