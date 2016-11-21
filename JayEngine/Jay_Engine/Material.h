#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Component.h"
#include "Color.h"
#include <vector>
#include <map>
#include <string>

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

	bool saveCMP(FileParser* sect);
	bool loadCMP(FileParser* sect);

private:

public:
	Color color;
	std::vector<uint> textures;
	std::map<std::string, int> paths;

private:
};

#endif // !__MATERIAL_H__