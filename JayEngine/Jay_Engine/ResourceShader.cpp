#include "ResourceShader.h"



ResourceShader::ResourceShader(uint UID) : Resource(UID, RESOURCE_SHADER)
{
}


ResourceShader::~ResourceShader()
{
}


bool ResourceShader::isShaderReady()
{
	return shaderID > 0;
}

bool ResourceShader::vertexShaderInMemory()
{
	return vertexShaderStr.empty();
}

bool ResourceShader::fragtalShaderInMemory()
{
	return fragtalShaderStr.empty();
}

bool ResourceShader::vertexAndFragtalInMemory()
{
	return vertexShaderInMemory() && fragtalShaderInMemory();
}

void ResourceShader::removeVertexShaderStr()
{
	vertexShaderStr.clear();
}

void ResourceShader::removeFragtalShaderStr()
{
	fragtalShaderStr.clear();
}

void ResourceShader::removeVertexAndFragtalShaderStr()
{
	removeVertexShaderStr(); removeFragtalShaderStr();
}