#include "JQuadTree.h"
#include "GameObject.h"

#define MAX_NODE_OBJECTS 8


//---------------------------------------------------
//---------------TreeNode----------------------------
//---------------------------------------------------


treeNode::treeNode(const AABB& _box) : box(_box)
{
	childs[0] = childs[1] = childs[2] = childs[3] = parent = NULL;
}

treeNode::~treeNode()
{
}

void treeNode::insert(GameObject* obj)
{
	if (!obj)
		return;

	if (childs[0] == NULL && objects.size() < MAX_NODE_OBJECTS)
		objects.push_back(obj);
	else
	{
		if (childs[0] == NULL)
			divideNode();

		objects.push_back(obj);
		ajustNode();
	}
}

void treeNode::erase(GameObject* obj)
{
	std::list<GameObject*>::iterator it = objects.begin();
	while (it != objects.end())  //TODO: check for std find funcs
	{
		if ((*it) == obj)
			it = objects.erase(it);
		else
			++it;
	}

	if (childs[0] != NULL)
		for (uint i = 0; i < 4; ++i)
			if (childs[i])childs[i]->erase(obj);
}

void treeNode::coollectBoxes(std::vector<AABB>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it)->aabb);
	}

	for (uint i = 0; i < 4; ++i)
		if (childs[i] != NULL)
			childs[i]->coollectBoxes(vec);
}

void treeNode::coollectGO(std::vector<GameObject*>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it));
	}

	for (uint i = 0; i < 4; ++i)
		if (childs[i] != NULL)
			childs[i]->coollectGO(vec);
}

void treeNode::divideNode()
{
	float3 center = box.CenterPoint();
	float3 center2 = float3::zero;
	float3 size = box.Size();
	float3 size2(size.x * 0.5f, size.y, size.z + 0.5f);
	AABB tmp;

	float sx = size.x * 0.25f;
	float sz = size.z * 0.25f;

	//----------Top-right
	center2.Set(center.x + sx, center.y, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[0] = new treeNode(tmp);

	//----------Bottom-right
	center2.Set(center.x + sx, center.y, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[1] = new treeNode(tmp);

	//----------Bottom-left
	center2.Set(center.x - sx, center.y, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[2] = new treeNode(tmp);

	//----------Top-right
	center2.Set(center.x - sx, center.y, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[3] = new treeNode(tmp);
}

void treeNode::ajustNode()
{
	std::list<GameObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		GameObject* tmp = (*it);
		if (intersectsAllChilds(tmp->aabb))
			++it; //Let the object in parent if it intersects with all childs
		else
		{
			it = objects.erase(it);
			for (uint i = 0; i < 4; ++i)
				if (box.Intersects(tmp->aabb)) //box.MinimalEnclosingAABB().Intersects()
					insert(tmp);
		}
	}
}

bool treeNode::intersectsAllChilds(const AABB& _box)
{
	uint count = 0;

	for (uint i = 0; i < 4; ++i)
		if (box.Intersects(_box)) //box.MinimalEnclosingAABB().Intersects()
			++count;

	return count == 4;
}


//---------------------------------------------------
//---------------JQuadTree---------------------------
//---------------------------------------------------


JQuadTree::JQuadTree()
{
}


JQuadTree::~JQuadTree()
{
	clear();
}

void JQuadTree::insert(GameObject* obj)
{
	if (rootNode && obj)
		if (rootNode->box.Intersects(obj->aabb))
			rootNode->insert(obj);
}

void JQuadTree::erase(GameObject* obj)
{
	if (rootNode && obj)
		rootNode->insert(obj);
}

void JQuadTree::setRoot(const AABB& _box)
{
	if (rootNode)
		RELEASE(rootNode);

	rootNode = new treeNode(_box);
}

void JQuadTree::clear()
{

	if (rootNode)
		RELEASE(rootNode);
}