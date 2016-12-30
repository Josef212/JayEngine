#ifndef __RESOURCESHADER_H__
#define __RESOURCESHADER_H__

#include "Resource.h"
#include <string>

class ResourceShader : public Resource
{
public:
	ResourceShader(uint UID);
	virtual ~ResourceShader();

	bool isShaderReady();

	bool vertexShaderInMemory();
	bool fragtalShaderInMemory();
	bool vertexAndFragtalInMemory();

	void removeVertexShaderStr();
	void removeFragtalShaderStr();
	void removeVertexAndFragtalShaderStr();

private:


public:
	std::string vertexShaderStr;
	std::string fragtalShaderStr;

	std::string shaderName;

	uint shaderID = 0;

};

#endif // !__RESOURCESHADER_H__