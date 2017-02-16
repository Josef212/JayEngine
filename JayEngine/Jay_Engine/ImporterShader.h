/**
	Actually is more like a shader editor or manager that 
	loads a shader to edit it and compile it.
*/

#ifndef __IMPORTER_SHADER__
#define __IMPORTER_SHADER__

#include <string>

class ResourceShader;

class ImporterShader
{
public:
	ImporterShader();
	virtual ~ImporterShader();

	//This must only be called once, TODO: maybe do this on constructor and save the default shader id here
	uint LoadDefaultShader(ResourceShader* resShader);

	const char* GetDefaultVertexShaderStr()const;
	const char* GetDefaultFragtalShaderStr()const;

	void FreeShader(uint shaderId);

	void LoadShaderToMemory(ResourceShader* resShader);
	void SaveShader(ResourceShader* resShader);
	void CompileShader(ResourceShader* resShader);

	void FirstCompile(ResourceShader* resShader);

private:
	std::string defaultVertexShader;
	std::string defaultFragmentShader;

};

#endif // !__IMPORTER_SHADER__