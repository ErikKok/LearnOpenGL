#pragma once
#include "ElementBuffer.h"

#include <glad/glad.h>

#include <cassert>

ElementBuffer::ElementBuffer(const GLvoid* data, GLuint size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void ElementBuffer::bindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void ElementBuffer::unbindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//inline unsigned int ElementBuffer::getCount() const
//{
//	return m_count;
//}