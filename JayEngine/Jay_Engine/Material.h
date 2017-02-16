#ifndef __MATERIAL__
#define __MATERIAL__

#include "Component.h"
#include "Color.h"
#include <vector>
#include <map>
#include <string>

class ResourceTexture;
class FileParser;
class ResourceShader;

class Material : public Component
{
public:
	Material(GameObject* gObj, int id);
	virtual ~Material();

	void Init()override;
	void Update(float dt)override;
	void CleanUp()override;

	bool SaveCMP(FileParser& sect)override;
	bool LoadCMP(FileParser& sect)override;

	void SetResource(UID resUID)override;
	void ClearMaterial();

private:

public:
	Color color = White;

	ResourceTexture* textureResource = nullptr; //Vector if normal map, light map, etc.
	ResourceShader* shaderResource = nullptr;

private:
};

#endif // !__MATERIAL__