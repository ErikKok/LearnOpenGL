#pragma once
#include "VertexBuffer.h"

#include "Global.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(size_t size, const GLvoid* data)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLuint>(size), data, GL_STATIC_DRAW);
	Global::glCheckError();
	//std::println("CREATE VertexBuffer id: {}", m_id);
}

VertexBuffer::~VertexBuffer()
{
	std::println("DELETE deleteVertexBuffer id: {}", m_id);
	glDeleteBuffers(1, &m_id);
	Global::glCheckError();
}

void VertexBuffer::bindVertexBuffer() const
{
	std::println("BIND VertexBuffer id: {}", m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	Global::glCheckError();
}

void VertexBuffer::unbindVertexBuffer() const
{
	std::println("UNBIND VertexBuffer id: {}", m_id); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Global::glCheckError();
}