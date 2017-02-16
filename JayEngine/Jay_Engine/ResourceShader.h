#ifndef __RESOURCESHADER__
#define __RESOURCESHADER__

#include "Resource.h"
#include <string>

class ResourceShader : public Resource
{
public:
	ResourceShader(uint UID);
	virtual ~ResourceShader();

	bool IsShaderReady();

	bool VertexShaderInMemory();
	bool FragtalShaderInMemory();
	bool VertexAndFragtalInMemory();

	void RemoveVertexShaderStr();
	void RemoveFragtalShaderStr();
	void RemoveVertexAndFragtalShaderStr();

	void ApplydefaultShader();

private:


public:
	std::string vertexShaderStr;
	std::string fragtalShaderStr;

	uint shaderID = 0;

	std::string vertexFile;
	std::string fragtalFile;
	std::string shaderName;

};

#endif // !__RESOURCESHADER__