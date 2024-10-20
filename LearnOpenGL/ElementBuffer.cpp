#pragma once
#include "ElementBuffer.h"

#include "Global.h"

#include <print>

#include <glad/glad.h>

// size_t is return value of sizeof() and .size()
ElementBuffer::ElementBuffer(size_t size, const GLvoid* data)
	: m_count{ static_cast<GLuint>(size / sizeof(GLuint)) }
{
	glCreateBuffers(1, &m_id);
	glNamedBufferStorage(m_id, static_cast<GLuint>(size), data, GL_DYNAMIC_STORAGE_BIT);
	G::glCheckError();
	//std::println("CREATE ElementBuffer id: {}", m_id);
}

ElementBuffer::~ElementBuffer()
{
	std::println("DELETE ElementBuffer id: {}", m_id);
	glDeleteBuffers(1, &m_id);
	G::glCheckError();
}

//void ElementBuffer::bindElementBuffer() const
//{
//	std::println("BIND ElementBuffer id: {}", m_id);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
//	Global::glCheckError();
//}
//
//void ElementBuffer::unbindElementBuffer() const
//{
//	std::println("UNBIND ElementBuffer id: {}", m_id);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	Global::glCheckError();
//}