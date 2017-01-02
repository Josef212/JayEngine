#include "Application.h"

#include "ResourceShader.h"
#include "ModuleResourceManager.h"
#include "ImporterShader.h"



ResourceShader::ResourceShader(uint UID) : Resource(UID, RESOURCE_SHADER)
{
	shaderName.assign("New shader");
	vertexFile.assign(std::to_string(uuid));
	vertexFile.append(VERTEX_SHADER_EXTENSION);

	fragtalFile.assign(std::to_string(uuid));
	fragtalFile.append(FRAGTAL_SHADER_EXTENSION);
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
	return !vertexShaderStr.empty();
}

bool ResourceShader::fragtalShaderInMemory()
{
	return !fragtalShaderStr.empty();
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


void ResourceShader::applydefaultShader()
{
	vertexShaderStr = app->resourceManager->shaderImporter->getDefaultVertexShaderStr();
	fragtalShaderStr = app->resourceManager->shaderImporter->getDefaultFragtalShaderStr();
}