/**
	Actually is more like a shader editor or manager that 
	loads a shader to edit it and compile it.
*/

#ifndef __IMPORTERSHADER_H__
#define __IMPORTERSHADER_H__

#include "Importer.h"

#include <string>

class ResourceShader;

class ImporterShader : public Importer
{
public:
	ImporterShader();
	virtual ~ImporterShader();

	//This must only be called once, TODO: maybe do this on constructor and save the default shader id here
	uint loadDefaultShader(ResourceShader* resShader);

	const char* getDefaultVertexShaderStr()const;
	const char* getDefaultFragtalShaderStr()const;

	void freeShader(uint shaderId);

	void loadShaderToMemory(ResourceShader* resShader);
	void saveShader(ResourceShader* resShader);
	void compileShader(ResourceShader* resShader);

	void firstCompile(ResourceShader* resShader);

private:
	std::string defaultVertexShader;
	std::string defaultFragmentShader;

};

#endif // !__IMPORTERSHADER_H__