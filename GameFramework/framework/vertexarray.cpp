// COMP710 GP Framework 2024

// This include:
#include "vertexarray.h"

// Local includes:

// Library includes:
#include <glew.h>
#include <cassert>
#include <iostream>
	static const int xyzStride = 3 * sizeof(float); // XYZ
	static const int xyStride = 2 * sizeof(float); // XY
VertexArray::VertexArray(const float* pVertexData, unsigned int numVertices, const unsigned int* pIndexData, unsigned int numIndices, int iStride)
	: m_numVertices(numVertices)
	, m_numIndices(numIndices)
	, m_glVertexBuffer(0)
	, m_glIndexBuffer(0)
	, m_glVertexArray(0)
{
	const int stride = 5 * sizeof(float); // XYZUV
	assert(pVertexData);

	glGenVertexArrays(1, &m_glVertexArray);
	glBindVertexArray(m_glVertexArray);

	glGenBuffers(1, &m_glVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
	if (iStride == 5)
	{
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * stride, pVertexData, GL_STATIC_DRAW);
	}
	else if (iStride == 3)
	{
		//std::cout << m_numVertices << " " << stride << std::endl;
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * xyzStride, pVertexData, GL_DYNAMIC_DRAW);
	}
	else if (iStride == 2)
	{
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * xyStride, pVertexData, GL_DYNAMIC_DRAW);

	}
		
	if (pIndexData)
	{
		glGenBuffers(1, &m_glIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(unsigned int), pIndexData, GL_STATIC_DRAW);
	}

	// Layout: XYZ
	glEnableVertexAttribArray(0);
	if(iStride==5)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	else if (iStride == 3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, xyzStride, 0);
	else if (iStride == 2)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, xyStride, 0);

	// Layout: UV
	if (iStride == 5)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(float) * 3));
	}
	/*else if (iStride == 7)
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(3 * sizeof(float)));
	}*/
}
void VertexArray::dummy(const float* pVertexData, unsigned int numVertices, int iStride) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer); 
	glBufferData(GL_ARRAY_BUFFER, numVertices * iStride * sizeof(float), pVertexData, GL_DYNAMIC_DRAW);
}
//void VertexArray::CreateGridVertexArray()
//{
//	const int stride = 6 * sizeof(float); // XYZRGB
//
//	glGenVertexArrays(1, &m_glVertexArray);
//	glBindVertexArray(m_glVertexArray);
//
//	glGenBuffers(1, &m_glVertexBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, )
//
//	glDeleteBuffers(1, &m_glVertexBuffer);
//	glDeleteBuffers(1, &m_glIndexBuffer);
//	glDeleteVertexArrays(1, &m_glVertexArray);
//}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_glVertexBuffer);
	glDeleteBuffers(1, &m_glIndexBuffer);
	glDeleteVertexArrays(1, &m_glVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(m_glVertexArray);
}

unsigned int VertexArray::GetNumVertices() const
{
	return m_numVertices;
}

unsigned int VertexArray::GetNumIndices() const
{
	return m_numIndices;
}