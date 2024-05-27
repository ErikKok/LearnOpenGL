#pragma once
#include "ElementBuffer.h"

#include "Global.h"

#include <glad/glad.h>

#include <cassert>

ElementBuffer::ElementBuffer(size_t size, const GLvoid* data)
	: m_count{ static_cast<GLuint>(size / sizeof(GLuint)) }
{
	//glGenBuffers(1, &m_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(size), data, GL_STATIC_DRAW);
	Global::glCheckError();
	//std::println("CREATE ElementBuffer id: {}", m_id);

	// DSA
	glCreateBuffers(1, &m_id);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); // TODO weg
	glNamedBufferStorage(m_id, static_cast<GLuint>(size), data, GL_DYNAMIC_STORAGE_BIT);
	Global::glCheckError();
}

ElementBuffer::~ElementBuffer()
{
	std::println("DELETE ElementBuffer id: {}", m_id);
	glDeleteBuffers(1, &m_id);
	Global::glCheckError();
}

void ElementBuffer::bindElementBuffer() const
{
	std::println("BIND ElementBuffer id: {}", m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	Global::glCheckError();
}

void ElementBuffer::unbindElementBuffer() const
{
	std::println("UNBIND ElementBuffer id: {}", m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Global::glCheckError();
}