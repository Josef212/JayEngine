#include "Component.h"

Component::Component(GameObject* gObj) : object(gObj)
{}

Component::~Component()
{}

//--------------------------------------------------

bool Component::IsActive()const
{
	return selfActive;
}

void Component::Enable()
{
	if (!selfActive)
	{
		selfActive = true;
		OnEnable();
	}
}

void Component::Disable()
{
	if (selfActive)
	{
		selfActive = false;
		OnDisable();
	}
}

void Component::SwitchActive()
{
	selfActive = !selfActive;

	if (selfActive)
		OnEnable();
	else
		OnDisable();
}

void Component::SetSelfActive(bool set)
{
	if (set != selfActive)
	{
		selfActive = set;
		if (set)
			OnEnable();
		else
			OnDisable();
	}
}

void Component::Destroy()
{
	removeFlag = true;
}

//--------------------------------------------------

ComponentType Component::GetType()const
{
	return type;
}

const char* Component::GetTypeStr()const
{
	static char* typesStr[] = {
		"Transform",
		"Mesh",
		"Material",
		"Camera",
		"SpriteRenderer",
		"Map"
	};

	if(type > CMP_UNKNOWN)
	{
		return typesStr[type];
	}
	else
	{
		return "Invalid type.";
	}
}

GameObject* Component::GetGameObject()const
{
	return object;
}
