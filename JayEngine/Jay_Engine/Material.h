#ifndef __MATERIAL__
#define __MATERIAL__

#include "Component.h"
#include "ComponentResource.h"
#include "Color.h"
#include <vector>
#include <map>
#include <string>

class ResourceTexture;
class FileParser;
class ResourceShader;

class Material : public Component, public ComponentResource
{
public:
	Material(GameObject* gObj);
	virtual ~Material();

	bool SaveCMP(FileParser& sect)const override;
	bool LoadCMP(FileParser* sect)override;

	bool SetResource(UID resUID)override;
	void ClearMaterial();

private:

public:
	Color color = White;
	ResourceShader* shaderResource = nullptr;

private:
};

#endif // !__MATERIAL__