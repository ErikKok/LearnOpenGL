#pragma once
#include "ElementBuffer.h"

#include <glad/glad.h>

#include <cassert>

ElementBuffer::ElementBuffer(const GLvoid* data, GLuint size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void ElementBuffer::bindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void ElementBuffer::unbindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//inline unsigned int ElementBuffer::getCount() const
//{
//	return m_Count;
//}