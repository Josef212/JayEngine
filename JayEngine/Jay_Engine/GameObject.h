#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Math.h"
#include <vector>
#include <string>

class Transform;
class Mesh;
class Material;
class Camera;

class FileParser;

class GameObject
{
public:
	GameObject(GameObject* parent, uint32 id);
	virtual ~GameObject();

	void init();
	void update(float dt);
	void cleanUp();

	void draw(bool drawChilds);

	Component* addComponent(ComponentType type);
	GameObject* addChild();
	bool removeComponent(Component* comp);
	std::vector<Component*> findComponent(ComponentType type);
	int hasComponent(ComponentType type);

	GameObject* getParent() const;
	int getGOId()const;
	const char* getName()const;
	void setName(const char* str);

	Transform* getTransform()const
	{
		return transform;
	}

	bool isGOActive();
	void setGOEnable(bool set);

	void drawDebug();

	void updateAABB();

	//TMP
	bool saveGO(FileParser* file);
	bool loadGO(FileParser* file);

private:


public:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;

	AABB enclosingBox;
	OBB orientedBox;
	bool drawEnclosingAABB = false;
	bool drawOrientedBox = false;

	bool toDel = false;

private:
	std::string name;
	int nextCompId = 0;
	uint32 id = 0;
	GameObject* parent = NULL;
	bool goActive = true;

	/**Just a pointer to transformation component 
	in order to agile transform search. May be transform info should be in game object class */
	Transform* transform = NULL;
};

#endif // !__GAMEOBJECT_H__