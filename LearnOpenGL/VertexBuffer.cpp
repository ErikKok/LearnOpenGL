#pragma once
#include "VertexBuffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const GLvoid* data, GLuint size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

void VertexBuffer::bindVertexBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::unbindVertexBuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}