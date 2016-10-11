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
	GameObject(GameObject* parent);
	virtual ~GameObject();

	void init();
	void update(float dt);
	void cleanUp();

	Component* addComponent(ComponentType type);
	bool removeComponent(Component* comp);
	//std::vector<Component*> findComponent(ComponentType type);
	Component* findComponent(ComponentType type);

	//const std::vector<Component*> getComponents()const;

	GameObject* getParent() const;

public:
	std::vector<Component*> components;

private:
	std::string name;
	int nextCompId = 0;

	GameObject* parent = NULL;
	std::vector<GameObject*> childrens;
};

#endif // !__GAMEOBJECT_H__