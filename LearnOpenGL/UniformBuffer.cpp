#pragma once
#include "UniformBuffer.h"
#include "Global.h"

#include <glad/glad.h>

#include <cassert>

UniformBuffer::UniformBuffer(GLuint index, GLint count)
{
	glm::mat4 type{}; // do I need other types here? If so: could be a in argument
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferData(GL_UNIFORM_BUFFER, count * sizeof(type), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, index, m_Id, 0, count * sizeof(type)); // always binds the whole buffer
	Global::glCheckError();
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_Id);
	Global::glCheckError();
}

void UniformBuffer::bindUniformBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	Global::glCheckError();
}

void UniformBuffer::unbindUniformBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	Global::glCheckError();
}

void UniformBuffer::addUniformBufferSubData(const UniformBuffer& ub, BufferSubDataLayout& layout)
{	
	ub.bindUniformBuffer();
	const auto& bufferSubData{ layout.getBufferSubData() };
	GLintptr totalOffset{ 0 };
	for (GLuint i{ 0 }; i < bufferSubData.size(); i++) {
		const auto& bufferSubDataElement{ bufferSubData[i] };
		assert(bufferSubDataElement.m_Target == GL_UNIFORM_BUFFER && "Wrong buffer binding target (must be GL_UNIFORM_BUFFER)");
		glBufferSubData(bufferSubDataElement.m_Target, totalOffset, bufferSubDataElement.m_Size, &bufferSubDataElement.m_Data);
		totalOffset += bufferSubDataElement.m_Size;
	}
	Global::glCheckError();
}