#pragma once
#include "ElementBuffer.h"

#include "Global.h"

#include <glad/glad.h>

#include <cassert>

ElementBuffer::ElementBuffer(const GLvoid* data, GLuint size) // the size in bytes of the buffer object's new data store.
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	Global::glCheckError();
	std::println("CREATE ElementBuffer id: {}", m_Id);
}

ElementBuffer::~ElementBuffer()
{
	std::println("DELETE ElementBuffer id: {}", m_Id);
	glDeleteBuffers(1, &m_Id);
	Global::glCheckError();
}

void ElementBuffer::bindElementBuffer() const
{
	std::println("BIND ElementBuffer id: {}", m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	Global::glCheckError();
}

void ElementBuffer::unbindElementBuffer() const
{
	std::println("UNBIND ElementBuffer id: {}", m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Global::glCheckError();
}