#pragma once
#include "ElementBuffer.h"

#include "Global.h"

#include <glad/glad.h>

#include <cassert>

ElementBuffer::ElementBuffer(const GLvoid* data, GLuint size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	Global::glCheckError();
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_Id);
	Global::glCheckError();
}

void ElementBuffer::bindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	Global::glCheckError();
}

void ElementBuffer::unbindElementBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Global::glCheckError();
}