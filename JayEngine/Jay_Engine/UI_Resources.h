#ifndef __UIRESOURCES_H__
#define __UIRESOURCES_H__

#include "UI_Comp.h"
#include <vector>

class Resource;
class ResourceShader;

class UI_Resources : public UI_Comp
{
public:
	UI_Resources();
	virtual ~UI_Resources();

	void draw();


private:
	void prefabs(std::vector<Resource*> prefs);
	void meshes(std::vector<Resource*> texs);
	void textures(std::vector<Resource*> texs);
	void shaders(std::vector<Resource*> shds);

	void shaderEditor(ResourceShader* resShader);


private:
	bool shaderEditorOpen = false;
	ResourceShader* currentShaderEditing = NULL;

	int infoW, infoH;
};

#endif // !__UIRESOURCES_H__