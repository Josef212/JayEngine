#include "JOctree.h"
#include "GameObject.h"

#define MAX_NODE_OBJECTS 8

//---------------------------------------------------
//---------------OcTreeNode----------------------------
//---------------------------------------------------


OcTreeNode::OcTreeNode(const AABB& _box) : box(_box)
{
	for (unsigned int i = 0; i < 8; ++i)
		childs[i] = nullptr;
	parent = nullptr;
}

OcTreeNode::~OcTreeNode()
{
}

void OcTreeNode::Insert(GameObject* obj)
{
	if (!obj)
		return;

	if (childs[0] == nullptr && objects.size() < MAX_NODE_OBJECTS)
		objects.push_back(obj);
	else
	{
		if (childs[0] == nullptr)
			DivideNode();

		objects.push_back(obj);
		AjustNode();
	}
}

void OcTreeNode::Erase(GameObject* obj)
{
	std::list<GameObject*>::iterator tmp = std::find(objects.begin(), objects.end(), obj);
	if (tmp != objects.end())
		objects.erase(tmp);

	if (childs[0] != nullptr)
		for (unsigned int i = 0; i < 8; ++i)
			if (childs[i])childs[i]->Erase(obj);
}

void OcTreeNode::CoollectBoxes(std::vector<AABB>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it)->enclosingBox);
	}

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i] != nullptr)
			childs[i]->CoollectBoxes(vec);
}

void OcTreeNode::CoollectGO(std::vector<GameObject*>& vec)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		vec.push_back((*it));
	}

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i] != nullptr)
			childs[i]->CoollectGO(vec);
}

void OcTreeNode::DivideNode()
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
	childs[0] = new OcTreeNode(tmp);

	//----------South-east
	center2.Set(center.x + sx, center.y + sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[1] = new OcTreeNode(tmp);

	//----------South-west
	center2.Set(center.x - sx, center.y + sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[2] = new OcTreeNode(tmp);

	//----------North-east
	center2.Set(center.x - sx, center.y + sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[3] = new OcTreeNode(tmp);

	//----------BOT----------
	//----------North-east
	center2.Set(center.x + sx, center.y - sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[4] = new OcTreeNode(tmp);

	//----------South-east
	center2.Set(center.x + sx, center.y - sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[5] = new OcTreeNode(tmp);

	//----------South-west
	center2.Set(center.x - sx, center.y - sy, center.z - sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[6] = new OcTreeNode(tmp);

	//----------North-east
	center2.Set(center.x - sx, center.y - sy, center.z + sz);
	tmp.SetFromCenterAndSize(center2, size2);
	childs[7] = new OcTreeNode(tmp);

	for (unsigned int i = 0; i < 8; ++i)
		childs[i]->parent = this;
}

void OcTreeNode::AjustNode()
{
	std::list<GameObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		GameObject* tmp = (*it);
		AABB b(tmp->enclosingBox);

		bool intersections[8];
		for (unsigned int i = 0; i < 8; ++i)
			intersections[i] = childs[i]->box.Intersects(b);

		if (intersections[0] && intersections[1] && intersections[2] && intersections[3] && intersections[4] && intersections[5] && intersections[6] && intersections[7])
		{
			++it; //Let the object in parent if it intersects with all childs
		}
		else
		{
			it = objects.erase(it);
			for (unsigned int i = 0; i < 8; ++i)
				if (childs[i]->box.Intersects(tmp->enclosingBox)) //box.MinimalEnclosingAABB().Intersects()
					childs[i]->Insert(tmp);
		}
	}
}

bool OcTreeNode::IntersectsAllChilds(const AABB& _box)
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i]->box.Intersects(_box)) //box.MinimalEnclosingAABB().Intersects()
			++count;

	return count == 4;
}

void OcTreeNode::CollectTreeBoxes(std::vector<AABB>& vec)
{
	vec.push_back(box);

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i]) childs[i]->CollectTreeBoxes(vec);
}

void OcTreeNode::CollectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (frustum.Intersects(box))
		for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
			if (frustum.Intersects((*it)->enclosingBox))
				vec.push_back((*it));

	for (unsigned int i = 0; i < 8; ++i)
		if (childs[i])childs[i]->CollectCandidates(vec, frustum);
}

//---------------------------------------------------
//---------------JOctree---------------------------
//---------------------------------------------------


JOctree::JOctree()
{
}


JOctree::~JOctree()
{
	Clear();
}

void JOctree::Insert(GameObject* obj)
{
	if (rootNode && obj)
		if (rootNode->box.Intersects(obj->enclosingBox))
			rootNode->Insert(obj);
}

void JOctree::Erase(GameObject* obj)
{
	if (rootNode && obj)
		rootNode->Erase(obj);
}

void JOctree::SetRoot(const AABB& _box)
{
	if (rootNode)
		delete(rootNode);

	rootNode = new OcTreeNode(_box);
}

void JOctree::Clear()
{

	if (rootNode)
		delete(rootNode);
	rootNode = nullptr;
}

void JOctree::CoollectBoxes(std::vector<AABB>& vec)
{
	if (rootNode)
		rootNode->CoollectBoxes(vec);
}

void JOctree::CoollectGO(std::vector<GameObject*>& vec)
{
	if (rootNode)
		rootNode->CoollectGO(vec);
}

void JOctree::CollectTreeBoxes(std::vector<AABB>& vec)
{
	if (rootNode)
		rootNode->CollectTreeBoxes(vec);
}

void JOctree::CollectCandidates(std::vector<GameObject*>& vec, const Frustum& frustum)
{
	if (rootNode)
		if (frustum.Intersects(rootNode->box))
			rootNode->CollectCandidates(vec, frustum);
}