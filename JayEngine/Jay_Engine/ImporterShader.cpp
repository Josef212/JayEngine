#include "Application.h"
#include "ImporterShader.h"

#include "ResourceShader.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"

#include "OpenGL.h"

ImporterShader::ImporterShader() : Importer()
{
	defaultVertexShader = std::string(
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec2 texCoord;\n"
		"layout (location = 2) in vec3 normal;\n\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform vec3 color;\n\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		"	ourColor = color;\n"
		"	TexCoord = texCoord;\n"
		"}\n"
	);

	defaultFragmentShader = std::string(
		"#version 330 core\n"
		"in vec3 ourColor;\n\n"
		"in vec2 TexCoord;\n\n"
		"out vec4 color;\n\n"
		"uniform sampler2D ourTexture;\n\n"
		"void main()\n"
		"{\n"
		"	color = texture(ourTexture, TexCoord);\n"
		"}\n"
	);
}


ImporterShader::~ImporterShader()
{
}

bool ImporterShader::LoadResource(Resource* resource)
{
	return false;
}

void ImporterShader::CompileShader(ResourceShader* resShader)
{
	if (!resShader)
		return;

	if (!resShader->VertexAndFragtalInMemory())
		LoadShaderToMemory(resShader);

	const char* str = resShader->vertexShaderStr.c_str();
	GLuint vS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vS, 1, &str, nullptr);
	glCompileShader(vS);

	GLint success;
	glGetShaderiv(vS, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vS, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Vertex shader compilation error: %s.", infoLog);
	}

	str = resShader->fragtalShaderStr.c_str();
	GLuint fS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fS, 1, &str, nullptr);
	glCompileShader(fS);

	glGetShaderiv(fS, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fS, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Fragment shader compilation error: %s.", infoLog);
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vS);
	glAttachShader(shaderProgram, fS);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
	}

	if (shaderProgram != 0 && success != 0)
		resShader->shaderID = shaderProgram;

	glDetachShader(shaderProgram, vS);
	glDetachShader(shaderProgram, fS);
	glDeleteShader(vS);
	glDeleteShader(fS);

	resShader->RemoveVertexAndFragtalShaderStr();
}

uint ImporterShader::LoadDefaultShader(ResourceShader* resShader)
{
	uint shaderRet = 0;

	if (!resShader)
		return shaderRet;

	const char* str = defaultVertexShader.c_str();
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &str, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Vertex shader compilation error: %s.", infoLog);
	}

	str = defaultFragmentShader.c_str();
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &str, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Fragment shader compilation error: %s.", infoLog);
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
	}

	if (shaderProgram != 0)
		shaderRet = shaderProgram;
	
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (shaderRet != 0)
		resShader->shaderID = shaderRet;
	else
		_LOG(LOG_WARN, "Could not load default shader.");

	return shaderRet;
}

const char* ImporterShader::GetDefaultVertexShaderStr()const
{
	return defaultVertexShader.c_str();
}

const char* ImporterShader::GetDefaultFragtalShaderStr()const
{
	return defaultFragmentShader.c_str();
}


void ImporterShader::FreeShader(uint shaderId)
{
	if (shaderId > 0)
		glDeleteProgram(shaderId);
}

void ImporterShader::FirstCompile(ResourceShader* resShader)
{
	if (!resShader)
		return;

	if (!resShader->IsInMemory())
		LoadShaderToMemory(resShader);

	CompileShader(resShader);

	resShader->RemoveVertexAndFragtalShaderStr();
}

void ImporterShader::LoadShaderToMemory(ResourceShader* resShader)
{
	if (!resShader)
		return;

	//First load vertex shader

	std::string path(PATH_SHADERS);
	path.append(resShader->vertexFile);

	char* buffer = nullptr;
	uint size = app->fs->Load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		resShader->vertexShaderStr = buffer;
		char* text = (char*)resShader->vertexShaderStr.c_str();
		text[size] = '\0';											//NOTE: This is a big **** but works to remove garbage. Cool for now
	}

	RELEASE_ARRAY(buffer);

	//Second fragtal shader

	path.assign(PATH_SHADERS);
	path.append(resShader->fragtalFile);

	size = 0;
	size = app->fs->Load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		resShader->fragtalShaderStr = buffer;
		char* text = (char*)resShader->fragtalShaderStr.c_str();
		text[size] = '\0';
	}

	RELEASE_ARRAY(buffer);
}

void ImporterShader::SaveShader(ResourceShader* resShader)
{
	if (!resShader)
		return;

	if (!resShader->VertexAndFragtalInMemory())
		return;

	std::string path(PATH_SHADERS);
	path.append(resShader->vertexFile);

	uint size = resShader->vertexShaderStr.size();
	if (app->fs->Save(path.c_str(), resShader->vertexShaderStr.c_str(), size) != size)
		_LOG(LOG_ERROR, "Error saving vertex shader.");

	size = 0;
	
	path.assign(PATH_SHADERS);
	path.append(resShader->fragtalFile);
	size = resShader->fragtalShaderStr.size();
	if(app->fs->Save(path.c_str(), resShader->fragtalShaderStr.c_str(), size) != size)
		_LOG(LOG_ERROR, "Error saving fragtal shader.");
}