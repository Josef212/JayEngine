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


bool ResourceShader::IsShaderReady()
{
	return shaderID > 0;
}

bool ResourceShader::VertexShaderInMemory()
{
	return !vertexShaderStr.empty();
}

bool ResourceShader::FragtalShaderInMemory()
{
	return !fragtalShaderStr.empty();
}

bool ResourceShader::VertexAndFragtalInMemory()
{
	return VertexShaderInMemory() && FragtalShaderInMemory();
}

void ResourceShader::RemoveVertexShaderStr()
{
	vertexShaderStr.clear();
}

void ResourceShader::RemoveFragtalShaderStr()
{
	fragtalShaderStr.clear();
}

void ResourceShader::RemoveVertexAndFragtalShaderStr()
{
	RemoveVertexShaderStr(); RemoveFragtalShaderStr();
}


void ResourceShader::ApplydefaultShader()
{
	vertexShaderStr = app->resourceManager->shaderImporter->GetDefaultVertexShaderStr();
	fragtalShaderStr = app->resourceManager->shaderImporter->GetDefaultFragtalShaderStr();
}