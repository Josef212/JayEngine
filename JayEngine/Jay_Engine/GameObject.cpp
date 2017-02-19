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
	transform = (Transform*)AddComponent(CMP_TRANSFORMATION);
}


GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); ++i)
		RELEASE(components[i]);

	for (uint j = 0; j < childs.size(); ++j)
		RELEASE(childs[j]);
}

void GameObject::Update(float dt)
{
	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->OnUpdate(dt);
	}
	for (uint i = 0; i < childs.size(); ++i)
	{
		childs[i]->Update(dt);
	}
}

//--------------------------------------------------------

void GameObject::OnStart()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnStart();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnStart();
}

void GameObject::OnFinish()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnFinish();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnFinish();
}

void GameObject::OnEnable()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnEnable();
}

void GameObject::OnDisable()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnDisable();
}

void GameObject::OnPlay()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnPlay();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnPlay();
}

void GameObject::OnStop()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnStop();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnStop();
}

void GameObject::OnPause()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnPause();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnPause();
}

void GameObject::OnUnPause()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnUnPause();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnUnPause();
}

void GameObject::OnGameObjectDestroyed()
{
	for (uint i = 0; i < components.size(); ++i)
		if (components[i])
			components[i]->OnGameObjectDestroyed();

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->OnGameObjectDestroyed();
}

//--------------------------------------------------------

UID GameObject::GetGOId()const
{
	return id;
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

//--------------------------------------------------------

GameObject* GameObject::AddChild()
{
	GameObject* ret = nullptr;

	ret = new GameObject(this, app->random->GetRandInt());
	childs.push_back(ret);

	return ret;
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
		if (!transform && !HasComponent(CMP_TRANSFORMATION))
		{
			ret = new Transform(this);
			transform = (Transform*)ret;
		}
		else
			_LOG(LOG_WARN, "Can't add transform component, already has one.");
	}
	break;

	case CMP_MESH:
	{
		ret = new Mesh(this);
	}
	break;

	case CMP_MATERIAL:
	{
		ret = new Material(this);
	}
	break;

	case CMP_CAMERA:
	{
		ret = new Camera(this);
	}
	break;

	//TODO: more cases
	}

	if (ret)
		components.push_back(ret);

	return ret;
}

Component* GameObject::GetComponent(ComponentType type)const
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->GetType() == type)
			return components[i];
	}

	return nullptr;
}

std::vector<Component*> GameObject::GetComponents(ComponentType type)
{
	std::vector<Component*> ret;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->GetType() == type)
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
		if (components[i]->GetType() == type)
			return true;
	}

	return false;
}

uint GameObject::CountComponents(ComponentType type)const
{
	int ret = 0;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == type)
			++ret;
	}

	return ret;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetNewParent(GameObject* newParent, bool force)
{
	if (newParent == parent)
		return;

	if (parent)
	{
		std::vector<GameObject*>::iterator it = std::find(parent->childs.begin(), parent->childs.end(), this);
		if (it != parent->childs.end())
			parent->childs.erase(it);
	}

	parent = newParent;

	if (newParent)
		newParent->childs.push_back(this);

	wasDirty = true;

	if (force && transform && newParent && newParent->transform)
	{
		float4x4 tmp = transform->GetGlobalTransform();
		transform->SetLocalTransform(tmp * newParent->transform->GetLocalTransform().Inverted());
	}
}

void GameObject::Draw(bool drawChilds) //Only use this if culling is not active
{
	if (!selfActive)
		return;

	app->renderer3D->DrawGameObject(this);

	if (drawChilds)
	{
		for (uint i = 0; i < childs.size(); ++i)
			if (childs[i])
				childs[i]->Draw(true);
	}
}

void GameObject::DrawDebug()
{
	for (uint j = 0; j < components.size(); ++j)
	{
		if (components[j])
			components[j]->OnDebugDraw();
	}

	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i])
			childs[i]->DrawDebug();
	}

	if (drawEnclosingAABB)
		DrawBoxDebug(enclosingBox, Green);
	if (drawOrientedBox)
		DrawBoxDebug(orientedBox, ClearBlue);
}

void GameObject::Remove()
{
	removeFlag = true;
}

bool GameObject::IsGOActive()
{
	return selfActive;
}

void GameObject::SetSelfActive(bool set) //TODO: Iteratie all childs and set same?
{
	if (set && !selfActive)
	{
		selfActive = true;
		OnEnable();
	}
	else if(!set && selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

void GameObject::Enable()//TODO: Iteratie all childs and set same?
{
	if (!selfActive)
		OnEnable();
}

void GameObject::Disable()//TODO: Iteratie all childs and set same?
{
	if(selfActive)
		OnDisable();
}

//--------------------------------------------------------

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

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i] && transform)
			childs[i]->RecCalcTransform(transform->GetGlobalTransform(), force);
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

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i] && transform)
			childs[i]->RecCalcBoxes();
}

void GameObject::RecalcBox()
{
	enclosingBox.SetNegativeInfinity();

	//Iterate all components and in each mesh add into AABB enclosing box all meshes aabb
	for (uint i = 0; i < components.size(); ++i)
	{
		Component* cmp = components[i];
		if (cmp && cmp->IsActive())
			cmp->GetBox(enclosingBox);
	}
}

bool GameObject::RecRemoveFlagged()
{
	bool ret = false;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i] && components[i]->removeFlag)
		{
			components[i]->OnFinish();
			RELEASE(components[i]);
			components.erase(components.begin() + i);
		}
	}
	
	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end();)
	{
		GameObject* tmp = *it;
		if (tmp && tmp->removeFlag)
		{
			tmp->OnFinish();
			app->goManager->EraseGameObjectFromTree(tmp);
			RELEASE(tmp);
			it = childs.erase(it);
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

//--------------------------------------------------------

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

	f.AddInt("UUID", uidToSave);
	f.AddInt("parent_id", parentID);

	f.AddString("name", name.c_str());

	f.AddArray("components");

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i])
		{
			FileParser cmp;
			components[i]->SaveCMP(cmp);
			f.AddArrayEntry(cmp);
		}
	}

	file.AddArrayEntry(f);

	for (uint i = 0; i < childs.size(); ++i)
		if (childs[i])
			childs[i]->SaveGO(file, duplicate);

	return ret;
}

bool GameObject::LoadGO(FileParser* file, std::map<GameObject*, uint>& relations)
{
	bool ret = true;

	id = file->GetInt("UUID", id);
	uint parent = file->GetInt("parent_id", 0);
	relations[this] = parent;

	name = file->GetString("name", "unnamed");

	int cmpCount = file->GetArraySize("components");
	for (uint i = 0; i < cmpCount; ++i)
	{
		FileParser cmp(file->GetArray("components", i));
		ComponentType type = (ComponentType)cmp.GetInt("comp_type", -1);
		if (type != CMP_UNKNOWN)
		{
			if (type == CMP_TRANSFORMATION)
				transform->LoadCMP(&cmp);
			else
			{
				Component* c = AddComponent(type);
				c->LoadCMP(&cmp);
			}
		}
		else
		{
			_LOG(LOG_ERROR, ("Unknown component type for game object %s.!!", name.c_str()));
		}
	}

	return ret;
}