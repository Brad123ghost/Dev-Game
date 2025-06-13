// COMP710 GP Framework 2024

// This include:
#include "shader.h"

// Local includes:
#include "logmanager.h"
#include "matrix4.h"

// Library includes:
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader()
	: m_vertexShader(0)
	, m_pixelShader(0)
	, m_geometryShader(0)
	, m_shaderProgram(0)
{

}

Shader::~Shader()
{
	Unload();
}

bool Shader::Load(const char* vertexFile, const char* pixelFile, const char* geomFile)
{
	bool vertexCompiled = CompileShader(vertexFile, GL_VERTEX_SHADER, m_vertexShader);
	bool pixelCompiled = CompileShader(pixelFile, GL_FRAGMENT_SHADER, m_pixelShader);
	if (geomFile[0] != '\0')
		CompileShader(geomFile, GL_GEOMETRY_SHADER, m_geometryShader);
		
	if (vertexCompiled == false || pixelCompiled == false)
	{
		LogManager::GetInstance().Log("[Shaders] Failed to compile!");
		assert(0);
		return false;
	}

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_pixelShader);
	if (geomFile[0] != '\0')
		glAttachShader(m_shaderProgram, m_geometryShader);
	glLinkProgram(m_shaderProgram);

	return IsValidProgram();
}

void Shader::Unload()
{
	if (m_vertexShader != 0)
	{
		glDeleteShader(m_vertexShader);
		m_vertexShader = 0;
	}
	if (m_pixelShader != 0)
	{
		glDeleteShader(m_pixelShader);
		m_pixelShader = 0;
	}
	if (m_geometryShader != 0)
	{
		glDeleteShader(m_geometryShader);
		m_geometryShader = 0;
	}
	if (m_shaderProgram != 0)
	{
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}
}

void Shader::SetActive()
{
	assert(m_shaderProgram);
	glUseProgram(m_shaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);

	glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&matrix);
}

void Shader::SetVector4Uniform(const char* name, float x, float y, float z, float w)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);

	float vec4[4];

	vec4[0] = x;
	vec4[1] = y;
	vec4[2] = z;
	vec4[3] = w;

	glUniform4fv(location, 1, vec4);
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);
	glUniform1f(location, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);
	glUniform1i(location, value);
}

bool Shader::CompileShader(const char* filename, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(filename);

	if (shaderFile.is_open())
	{
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string shaderCode = sstream.str();
		const char* pShaderCode = shaderCode.c_str();

		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(pShaderCode), 0);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			std::string logMsg = "[Shaders] " + std::string(filename) + " failed to compile!";
			LogManager::GetInstance().Log(logMsg.c_str());
			return false;
		}
	} 
	else
	{
		std::string logMsg = "[Shaders] " + std::string(filename) + " file not found!";
		LogManager::GetInstance().Log(logMsg.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint compiledStatus;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);

	if (compiledStatus != GL_TRUE)
	{
		char error[1024];
		error[0] = 0;
		glGetShaderInfoLog(shader, 1023, 0, error);

		std::string logMsg = "[Shaders] Failed to compile!";
		LogManager::GetInstance().Log(logMsg.c_str());

		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint linkedStatus;

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkedStatus);

	if (linkedStatus != GL_TRUE)
	{
		char error[1024];
		error[0] = 0;
		glGetShaderInfoLog(m_shaderProgram, 1023, 0, error);

		std::string logMsg = "[Shaders] Failed to link!";
		LogManager::GetInstance().Log(logMsg.c_str());
		assert(0);

		return false;
	}

	return true;
}