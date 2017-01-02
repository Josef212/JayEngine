#include "Application.h"
#include "ImporterShader.h"

#include "ResourceShader.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"

#include "OpenGL.h"

ImporterShader::ImporterShader()
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
		//"	color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);\n"
		//"	gl_FragColor = ourColor;\n"
		"	color = texture(ourTexture, TexCoord);\n"
		"}\n"
	);
}


ImporterShader::~ImporterShader()
{
}

void ImporterShader::compileShader(ResourceShader* resShader)
{
	if (!resShader)
		return;

	const char* str = resShader->vertexShaderStr.c_str();
	GLuint vS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vS, 1, &str, NULL);
	glCompileShader(vS);

	GLint success;
	glGetShaderiv(vS, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vS, 512, NULL, infoLog);
		_LOG(LOG_ERROR, "Vertex shader compilation error: %s.", infoLog);
	}

	str = resShader->fragtalShaderStr.c_str();
	GLuint fS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fS, 1, &str, NULL);
	glCompileShader(fS);

	glGetShaderiv(fS, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fS, 512, NULL, infoLog);
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
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
	}

	if (shaderProgram != 0)
		resShader->shaderID = shaderProgram;

	glDetachShader(shaderProgram, vS);
	glDetachShader(shaderProgram, fS);
	glDeleteShader(vS);
	glDeleteShader(fS);
}

uint ImporterShader::loadDefaultShader()
{
	uint shaderRet = 0;

	const char* str = defaultVertexShader.c_str();
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &str, NULL);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		_LOG(LOG_ERROR, "Vertex shader compilation error: %s.", infoLog);
	}

	str = defaultFragmentShader.c_str();
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &str, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		_LOG(LOG_ERROR, "Shader link error: %s.", infoLog);
	}

	if (shaderProgram != 0)
		shaderRet = shaderProgram;
	
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderRet;
}

const char* ImporterShader::getDefaultVertexShaderStr()const
{
	return defaultVertexShader.c_str();
}

const char* ImporterShader::getDefaultFragtalShaderStr()const
{
	return defaultFragmentShader.c_str();
}


void ImporterShader::freeShader(uint shaderId)
{
	if (shaderId > 0)
		glDeleteProgram(shaderId);
}

void ImporterShader::firstCompile(ResourceShader* resShader)
{
	if (!resShader)
		return;

	if (!resShader->isInMemory())
		loadShaderToMemory(resShader);

	compileShader(resShader);

	//TODO: Unload shaders from memory.
}

void ImporterShader::loadShaderToMemory(ResourceShader* resShader)
{
	if (!resShader)
		return;

	//First load vertex shader

	std::string path(DEFAULT_SHADER_SAVE_PATH);
	path.append(resShader->vertexFile);

	char* buffer = NULL;
	uint size = app->fs->load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		resShader->vertexShaderStr = buffer;
	}

	RELEASE_ARRAY(buffer);

	//Second fragtal shader

	path.assign(DEFAULT_SHADER_SAVE_PATH);
	path.append(resShader->fragtalFile);

	size = 0;
	size = app->fs->load(path.c_str(), &buffer);

	if (buffer && size > 0)
	{
		resShader->fragtalShaderStr = buffer;
	}

	RELEASE_ARRAY(buffer);
}

void ImporterShader::saveShader(ResourceShader* resShader)
{
	if (!resShader)
		return;

	if (!resShader->vertexAndFragtalInMemory())
		return;

	std::string path(DEFAULT_SHADER_SAVE_PATH);
	path.append(resShader->vertexFile);

	uint size = resShader->vertexShaderStr.size();
	if (app->fs->save(path.c_str(), resShader->vertexShaderStr.c_str(), size) != size)
		_LOG(LOG_ERROR, "Error saving vertex shader.");

	size = 0;
	
	path.assign(DEFAULT_SHADER_SAVE_PATH);
	path.append(resShader->fragtalFile);
	size = resShader->fragtalShaderStr.size();
	if(app->fs->save(path.c_str(), resShader->fragtalShaderStr.c_str(), size) != size)
		_LOG(LOG_ERROR, "Error saving fragtal shader.");
}