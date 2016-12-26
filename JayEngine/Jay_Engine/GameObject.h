#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "Math.h"
#include <vector>
#include <string>
#include <map>

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
	void update(float dt); //Only called on play mode
	void cleanUp();

	void draw(bool drawChilds);

	Component* addComponent(ComponentType type);
	GameObject* addChild();

	std::vector<Component*> findComponent(ComponentType type);
	int hasComponent(ComponentType type);

	GameObject* getParent() const;
	void setNewParent(GameObject* newParent, bool force = false);

	int getGOId()const;
	const char* getName()const;
	void setName(const char* str);

	void remove();

	bool isGOActive();
	void setGOEnable(bool set);

	void drawDebug();

	void recCalcTransform(const float4x4& parentTrans, bool force = false);
	void recCalcBoxes();
	void recalcBox();
	bool recRemoveFlagged();

	void onGameObjectDestroyed();

	//TMP
	bool saveGO(FileParser& file, std::map<uint, uint>* duplicate = NULL)const;
	bool loadGO(FileParser* file, std::map<GameObject*, uint>& relations);

private:


public:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;

	AABB enclosingBox;
	OBB orientedBox;
	bool drawEnclosingAABB = false;
	bool drawOrientedBox = false;

	bool removeFlag = false;

	/**Just a pointer to transformation component
	in order to agile transform search. May be transform info should be in game object class */
	Transform* transform = NULL;

private:
	std::string name;
	int nextCompId = 0;
	uint32 id = 0;
	GameObject* parent = NULL;
	bool goActive = true;

	bool goWasDirty = true;

};

#endif // !__GAMEOBJECT_H__