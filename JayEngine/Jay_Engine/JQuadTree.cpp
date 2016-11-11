#include "JQuadTree.h"
#include "GameObject.h"

#define MAX_NODE_OBJECTS 8


//---------------------------------------------------
//---------------TreeNode----------------------------
//---------------------------------------------------


treeNode::treeNode(const AABB& _box) : box(_box)
{
	childs[0] = childs[1] = childs[2] = childs[3] = parent = nullptr;
}

treeNode::~treeNode()
{
}

void treeNode::insert(GameObject* obj)
{
	if (!obj)
		return;

	if (childs[0] == nullptr && objects.size() < MAX_NODE_OBJECTS)
		objects.push_back(obj);
	else
	{
		if (childs[0] == nullptr)
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

	if (childs[0] != nullptr)
		for (unsigned int i = 0; i < 4; ++i)
			if (childs[i])childs[i]->erase(obj);
}

void treeNode::coollectBoxes(std::vector<AABB>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it)->enclosingBox);
	}

	for (unsigned int i = 0; i < 4; ++i)
		if (childs[i] != nullptr)
			childs[i]->coollectBoxes(vec);
}

void treeNode::coollectGO(std::vector<GameObject*>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it));
	}

	for (unsigned int i = 0; i < 4; ++i)
		if (childs[i] != nullptr)
			childs[i]->coollectGO(vec);
}

void treeNode::divideNode()
{
	float3 center = box.CenterPoint();
	float3 center2 = float3::zero;
	float3 size = box.Size();
	float3 size2(size.x * 0.5f, size.y, size.z * 0.5f);
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

	for (unsigned int i = 0; i < 4; ++i)
		childs[i]->parent = this;
}

void treeNode::ajustNode()
{
	std::list<GameObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		GameObject* tmp = (*it);
		if (intersectsAllChilds(tmp->enclosingBox))
			++it; //Let the object in parent if it intersects with all childs
		else
		{
			it = objects.erase(it);
			for (unsigned int i = 0; i < 4; ++i)
				if (childs[i]->box.Intersects(tmp->enclosingBox)) //box.MinimalEnclosingAABB().Intersects()
					childs[i]->insert(tmp);
		}
	}
}

bool treeNode::intersectsAllChilds(const AABB& _box)
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < 4; ++i)
		if (childs[i]->box.Intersects(_box)) //box.MinimalEnclosingAABB().Intersects()
			++count;

	return count == 4;
}

void treeNode::collectTreeBoxes(std::vector<AABB>& vec)
{
	vec.push_back(box);

	for (unsigned int i = 0; i < 4; ++i)
		if (childs[i]) childs[i]->collectTreeBoxes(vec);
}

void treeNode::collectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (frustum.Intersects(box))
		for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
			if (frustum.Intersects((*it)->enclosingBox))
				vec.push_back((*it));

	for (unsigned int i = 0; i < 4; ++i)
		if (childs[i])childs[i]->collectCandidates(vec, frustum);
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
		if (rootNode->box.Intersects(obj->enclosingBox))
			rootNode->insert(obj);
}

void JQuadTree::erase(GameObject* obj)
{
	if (rootNode && obj)
		rootNode->erase(obj);
}

void JQuadTree::setRoot(const AABB& _box)
{
	if (rootNode)
		delete(rootNode);

	rootNode = new treeNode(_box);
}

void JQuadTree::clear()
{

	if (rootNode)
		delete(rootNode);
	rootNode = nullptr;
}

void JQuadTree::coollectBoxes(std::vector<AABB>& vec)
{
	if (rootNode)
		rootNode->coollectBoxes(vec);
}

void JQuadTree::coollectGO(std::vector<GameObject*>& vec)
{
	if (rootNode)
		rootNode->coollectGO(vec);
}

void JQuadTree::collectTreeBoxes(std::vector<AABB>& vec)
{
	if (rootNode)
		rootNode->collectTreeBoxes(vec);
}

void JQuadTree::collectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (rootNode)
		if (frustum.Intersects(rootNode->box))
			rootNode->collectCandidates(vec, frustum);
}