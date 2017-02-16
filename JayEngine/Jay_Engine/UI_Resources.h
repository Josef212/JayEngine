#ifndef __UI_RESOURCES__
#define __UI_RESOURCES__

#include "UI_Panel.h"
#include <vector>

class Resource;
class ResourceShader;

class UI_Resources : public UI_Panel
{
public:
	UI_Resources(bool startEnalbed = false);
	virtual ~UI_Resources();

	void Draw()override;


private:
	void Prefabs(std::vector<Resource*> prefs);
	void Meshes(std::vector<Resource*> texs);
	void Textures(std::vector<Resource*> texs);
	void Shaders(std::vector<Resource*> shds);

	void ShaderEditor(ResourceShader* resShader);


private:
	bool shaderEditorOpen = false;
	ResourceShader* currentShaderEditing = NULL;

	int infoW, infoH;
};

#endif // !__UI_RESOURCES__