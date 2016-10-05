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
	GameObject();
	virtual ~GameObject();

	void init();
	void update(float dt);
	void cleanUp();

	Component* addComponent(ComponentType type);
	bool removeComponent(Component* comp);
	std::vector<Component*> findComponent(ComponentType type);

private:
	std::string name;

	GameObject* parent = NULL;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
};

#endif // !__GAMEOBJECT_H__