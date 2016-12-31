#include "ImporterShader.h"

#include "OpenGL.h"

ImporterShader::ImporterShader()
{
	defaultVertexShader = std::string(
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec2 texCoord;\n"
		"layout (location = 2) in vec3 normal;\n\n"
		//"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		//"	ourColor = color;\n"
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