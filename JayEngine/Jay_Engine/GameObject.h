#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>

class Transform;
class Mesh;
class Material;

class GameObject
{
public:
	GameObject(GameObject* parent, int id);
	virtual ~GameObject();

	void init();
	void update(float dt);
	void cleanUp();

	Component* addComponent(ComponentType type);
	GameObject* addChild();
	bool removeComponent(Component* comp);
	//std::vector<Component*> findComponent(ComponentType type);
	Component* findComponent(ComponentType type);

	//const std::vector<Component*> getComponents()const;

	GameObject* getParent() const;
	int getGOId()const;
	const char* getName()const;
	void setName(const char* str);

public:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;

private:
	std::string name;
	int nextCompId = 0;
	int id = -1;
	GameObject* parent = NULL;
};

#endif // !__GAMEOBJECT_H__