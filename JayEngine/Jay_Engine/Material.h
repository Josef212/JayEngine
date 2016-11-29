#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "Color.h"
#include <vector>
#include <map>
#include <string>

class ResourceTexture;
class FileParser;

class Material : public Component
{
public:
	Material(GameObject* gObj, int id);
	virtual ~Material();

	void enable();
	void disable();

	void init();
	void update(float dt);
	void cleanUp();

	int loadTexture(char* file, char* path = NULL);
	int getTexture(int index);
	uint getTexxturesSize()
	{
		return textures.size();
	}

	ResourceTexture* createAnEmptyMaterialRes();

	bool saveCMP(FileParser& sect);
	bool loadCMP(FileParser& sect);

private:

public:
	Color color;
	std::vector<uint> textures;
	std::map<std::string, int> paths;

	ResourceTexture* textureResource = NULL; //Vector if normal map, light map, etc.

private:
};

#endif // !__MATERIAL_H__