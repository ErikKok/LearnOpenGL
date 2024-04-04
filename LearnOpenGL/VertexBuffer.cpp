#pragma once
#include "VertexBuffer.h"

#include "Global.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const GLvoid* data, GLuint size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	Global::glCheckError();
	std::println("CREATE VertexBuffer id: {}", m_Id);
}

VertexBuffer::~VertexBuffer()
{
	std::println("DELETE deleteVertexBuffer id: {}", m_Id);
	glDeleteBuffers(1, &m_Id);
	Global::glCheckError();
}

void VertexBuffer::bindVertexBuffer() const
{
	std::println("BIND VertexBuffer id: {}", m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	Global::glCheckError();
}

void VertexBuffer::unbindVertexBuffer() const
{
	std::println("UNBIND VertexBuffer id: {}", m_Id); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Global::glCheckError();
}