// COMP710 GP Framework 2024
#ifndef __SHADER_H_
#define __SHADER_H_

#include "glew.h"
//#include "matrix4.h"
// Forward declarations:
struct Matrix4;

class Shader
{
	// Member methods:
public:
	Shader();
	~Shader();

	bool Load(const char* vertexFile, const char* pixelFile, const char* geomFile = "\0");
	void Unload();

	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVector4Uniform(const char* name, float x, float y, float z, float w);
	void SetFloatUniform(const char* name, float value);
	void SetIntUniform(const char* name, int value);

protected:

private:
	bool IsValidProgram();

	static bool CompileShader(const char* filename, GLenum shaderType, GLuint& outShader);
	static bool IsCompiled(GLuint shader);

	// Member data:
public:

protected:
	GLuint m_vertexShader;
	GLuint m_pixelShader;
	GLuint m_geometryShader;
	GLuint m_shaderProgram;

private:

};

#endif // __SHADER_H_