#include "JOcttree.h"
#include "GameObject.h"

#define MAX_NODE_OBJECTS 8

//---------------------------------------------------
//---------------TreeNode----------------------------
//---------------------------------------------------


oTreeNode::oTreeNode(const AABB& _box) : box(_box)
{
	parent = nullptr;
	for (unsigned int i = 0; i < 8; ++i)
		childs[i] = nullptr;
}

oTreeNode::~oTreeNode()
{
}

void oTreeNode::insert(GameObject* obj)
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

void oTreeNode::erase(GameObject* obj)
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
		for (unsigned int i = 0; i < 8; ++i)
			if (childs[i])childs[i]->erase(obj);
}

void oTreeNode::coollectBoxes(std::vector<AABB>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it)->enclosingBox);
	}

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i] != nullptr)
			childs[i]->coollectBoxes(vec);
}

void oTreeNode::coollectGO(std::vector<GameObject*>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it));
	}

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i] != nullptr)
			childs[i]->coollectGO(vec);
}

void oTreeNode::divideNode()
{
	float3 center = box.CenterPoint();
	float3 center2 = float3::zero;
	float3 size = box.Size();
	float3 size2(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	AABB tmp;

	float sx = size.x * 0.25f;
	float sy = size.y * 0.25f;
	float sz = size.z * 0.25f;

	//----------TOP----------
	//----------North-east
	center2.Set(center.x + sx, center.y + sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[0] = new oTreeNode(tmp);

	//----------South-east
	center2.Set(center.x + sx, center.y + sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[1] = new oTreeNode(tmp);

	//----------South-west
	center2.Set(center.x - sx, center.y + sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[2] = new oTreeNode(tmp);

	//----------North-east
	center2.Set(center.x - sx, center.y + sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[3] = new oTreeNode(tmp);

	//----------BOT----------
	//----------North-east
	center2.Set(center.x + sx, center.y - sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[4] = new oTreeNode(tmp);

	//----------South-east
	center2.Set(center.x + sx, center.y - sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[5] = new oTreeNode(tmp);

	//----------South-west
	center2.Set(center.x - sx, center.y - sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[6] = new oTreeNode(tmp);

	//----------North-east
	center2.Set(center.x - sx, center.y - sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[7] = new oTreeNode(tmp);

	for (unsigned int i = 0; i < 8; ++i)
		childs[i]->parent = this;
}

void oTreeNode::ajustNode()
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
			for (unsigned int i = 0; i < 8; ++i)
				if (box.Intersects(tmp->enclosingBox)) //box.MinimalEnclosingAABB().Intersects()
					insert(tmp);
		}
	}
}

bool oTreeNode::intersectsAllChilds(const AABB& _box)
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < 8; ++i)
		if (box.Intersects(_box)) //box.MinimalEnclosingAABB().Intersects()
			++count;

	return count == 8;
}

void oTreeNode::collectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (frustum.Intersects(box))
		for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
			if (frustum.Intersects((*it)->enclosingBox))
				vec.push_back((*it));

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i])childs[i]->collectCandidates(vec, frustum);
}


//---------------------------------------------------
//---------------JOctTree---------------------------
//---------------------------------------------------


JOctTree::JOctTree()
{
}


JOctTree::~JOctTree()
{
	clear();
}

void JOctTree::insert(GameObject* obj)
{
	if (rootNode && obj)
		if (rootNode->box.Intersects(obj->enclosingBox)) //box.MinimalEnclosingAABB().Intersects()
			rootNode->insert(obj);
}

void JOctTree::erase(GameObject* obj)
{
	if (rootNode && obj)
		rootNode->insert(obj);
}

void JOctTree::setRoot(const AABB& _box)
{
	if (rootNode)
		delete(rootNode);

	rootNode = new oTreeNode(_box);
}

void JOctTree::clear()
{

	if (rootNode)
		delete(rootNode);
	rootNode = nullptr;
}

void JOctTree::collectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (rootNode)
		if (frustum.Intersects(rootNode->box))
			rootNode->collectCandidates(vec, frustum);
}