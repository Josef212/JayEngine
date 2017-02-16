#include "Application.h"
#include "GameObject.h"

#include "DrawDebug.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"
#include "RandGen.h"

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ResourceMesh.h"

#include "ModuleGOManager.h"

//TMP
#include "FileParser.h"


GameObject::GameObject(GameObject* parent, UID id) : parent(parent), id(id)
{
	name.assign("Game Object");
	Init();
	transform = (Transform*)AddComponent(CMP_TRANSFORMATION);
}


GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); ++i)
		RELEASE(components[i]);

	for (uint j = 0; j < childrens.size(); ++j)
		RELEASE(childrens[j]);
}

void GameObject::Init()
{
}

void GameObject::Update(float dt)
{
	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->Update(dt);
	}
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->Update(dt);
	}
}

void GameObject::CleanUp()
{
	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->CleanUp();
	}

	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->CleanUp();
	}
}

const char* GameObject::GetName()const
{
	return name.c_str();
}

void GameObject::SetName(const char* str)
{
	if (str)
		name.assign(str);
}

UID GameObject::GetGOId()const
{
	return id;
}

bool GameObject::IsGOActive()
{
	return selfActive;
}

void GameObject::SetSelfActive(bool set)
{
	selfActive = set;
}

Component* GameObject::AddComponent(ComponentType type)
{
	Component* ret = nullptr;

	switch (type)
	{
		case CMP_UNKNOWN:
			_LOG(LOG_ERROR, "Error while creating component, no component type.");
		break;

		case CMP_TRANSFORMATION:
		{
			if (!HasComponent(CMP_TRANSFORMATION))
			{
				ret = new Transform(this, nextCompId);
				transform = (Transform*)ret;
			}
			else
				_LOG(LOG_WARN, "Can't add transform component, already has one.");
		}
		break;

		case CMP_MESH:
		{
			ret = new Mesh(this, nextCompId);
		}
		break;

		case CMP_MATERIAL:
		{
			ret = new Material(this, nextCompId);
		}
		break;

		case CMP_CAMERA:
		{
			ret = new Camera(this, nextCompId);
		}
		break;

		//TODO: more cases
	}

	if (ret)
	{
		++nextCompId;
		components.push_back(ret);
	}

	return ret;
}

GameObject* GameObject::AddChild()
{
	GameObject* ret = nullptr;

	ret = new GameObject(this, app->random->GetRandInt());
	childrens.push_back(ret);

	return ret;
}

void GameObject::Remove()
{
	removeFlag = true;
}

bool GameObject::RecRemoveFlagged()
{
	bool ret = false;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->removeFlag)
		{
			components[i]->CleanUp();
			RELEASE(components[i]);
			components.erase(components.begin() + i);
		}
	}
	
	for (std::vector<GameObject*>::iterator it = childrens.begin(); it != childrens.end();)
	{
		GameObject* tmp = *it;
		if (tmp && tmp->removeFlag)
		{
			tmp->CleanUp();
			app->goManager->EraseGameObjectFromTree(tmp);
			RELEASE(tmp);
			it = childrens.erase(it);
			ret = true;
		}
		else
		{
			ret |= tmp->RecRemoveFlagged();
			++it;
		}
	}

	return ret;
}

void GameObject::OnGameObjectDestroyed()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnGameObjectDestroyed();
}

Component* GameObject::GetComponent(ComponentType type)const
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->type == type)
			return components[i];
	}

	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(ComponentType type)
{
	std::vector<Component*> ret;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->type == type)
			ret.push_back(components[i]);
	}

	if (ret.empty())
		ret.push_back(nullptr);

	return ret;
}

bool GameObject::HasComponent(ComponentType type)const
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
			return true;
	}

	return false;
}

uint GameObject::CountComponents(ComponentType type)const
{
	int ret = 0;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
			++ret;
	}

	return ret;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

void GameObject::Draw(bool drawChilds) //Only use this if culling is not active
{
	if (!selfActive)
		return;

	app->renderer3D->DrawGameObject(this);
		
	if (drawChilds)
	{
		for (uint i = 0; i < childrens.size(); ++i)
			if (childrens[i])
				childrens[i]->Draw(true);
	}
}

void GameObject::DrawDebug()
{
	for (uint j = 0; j < components.size(); ++j)
	{
		if (components[j])
			components[j]->DebugDraw();
	}

	for (uint i = 0; i < childrens.size(); ++i)
	{
		if(childrens[i])
			childrens[i]->DrawDebug();
	}

	if (drawEnclosingAABB)
		DrawBoxDebug(enclosingBox, Green);
	if (drawOrientedBox)
		DrawBoxDebug(orientedBox, ClearBlue);
}

void GameObject::RecCalcTransform(const float4x4& parentTrans, bool force)
{
	if (transform && transform->localTransformChanged || force)
	{
		force = true;
		wasDirty = true;
		transform->UpdateTransform(parentTrans);

		for (uint i = 0; i < components.size(); ++i)
			if (components[i])
				components[i]->OnTransformUpdate(transform);

		app->goManager->EraseGameObjectFromTree(this);
		app->goManager->InsertGameObjectToTree(this);
	}
	else
		wasDirty = false;

	for (uint i = 0; i < childrens.size(); ++i)
		if (childrens[i] && transform)
			childrens[i]->RecCalcTransform(transform->GetGlobalTransform(), force);
}

void GameObject::RecCalcBoxes()
{
	if (wasDirty)
	{
		RecalcBox();

		orientedBox = enclosingBox;
		if (orientedBox.IsFinite() && transform)
		{
			orientedBox.Transform(transform->GetGlobalTransform());
			enclosingBox.SetFrom(orientedBox);
		}
	}

	for (uint i = 0; i < childrens.size(); ++i)
		if (childrens[i] && transform)
			childrens[i]->RecCalcBoxes();
}

void GameObject::RecalcBox()
{
	enclosingBox.SetNegativeInfinity();

	//Iterate all components and in each mesh add into AABB enclosing box all meshes aabb
	for (uint i = 0; i < components.size(); ++i)
	{
		Component* cmp = components[i];
		if (cmp && cmp->IsEnable())
			cmp->GetBox(enclosingBox);
	}
}

void GameObject::SetNewParent(GameObject* newParent, bool force)
{
	if (newParent == parent)
		return;

	if (parent)
	{
		std::vector<GameObject*>::iterator it = std::find(parent->childrens.begin(), parent->childrens.end(), this);
		if (it != parent->childrens.end())
			parent->childrens.erase(it);
	}

	parent = newParent;

	if (newParent)
		newParent->childrens.push_back(this);

	wasDirty = true;

	if (force && transform && newParent && newParent->transform)
	{
		float4x4 tmp = transform->GetGlobalTransform();
		transform->SetLocalTransform(tmp * newParent->transform->GetLocalTransform().Inverted());
	}
}

bool GameObject::SaveGO(FileParser& file, std::map<uint, uint>* duplicate)const
{
	bool ret = true;

	FileParser f;

	uint uidToSave = id;
	uint parentID = (parent) ? parent->GetGOId() : 0;

	if (duplicate)
	{
		uidToSave = app->random->GetRandInt();
		(*duplicate)[id] = uidToSave;

		std::map<uint, uint>::iterator it = duplicate->find(parentID);
		if (it != duplicate->end())
			parentID = it->second;
	}

	f.addInt("UUID", id);
	f.addInt("parent_id", parentID);

	f.addString("name", name.c_str());

	f.addArray("components");

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i])
		{
			FileParser cmp;
			components[i]->SaveCMP(cmp);
			f.addArrayEntry(cmp);
		}
	}

	file.addArrayEntry(f);

	for (uint i = 0; i < childrens.size(); ++i)
	{
		if (childrens[i])
			childrens[i]->SaveGO(file, duplicate);
	}

	return ret;
}

bool GameObject::LoadGO(FileParser* file, std::map<GameObject*, uint>& relations)
{
	bool ret = true;

	id = file->getInt("UUID", id);
	uint parent = file->getInt("parent_id", 0);
	relations[this] = parent;

	name = file->getString("name", "unnamed");

	int cmpCount = file->getArraySize("components");
	for (uint i = 0; i < cmpCount; ++i)
	{
		FileParser cmp(file->getArray("components", i));
		ComponentType type = (ComponentType)cmp.getInt("comp_type", -1);
		if (type != CMP_UNKNOWN)
		{
			if (type == CMP_TRANSFORMATION)
				transform->LoadCMP(cmp);
			else
			{
				Component* c = AddComponent(type);
				c->LoadCMP(cmp);
			}
		}
		else
			_LOG(LOG_ERROR, ("Unknown component type for game object %s.!!", name.c_str()));
	}

	return ret;
}