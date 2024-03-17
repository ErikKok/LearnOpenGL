#pragma once
#include "UniformBuffer.h"

#include <glad/glad.h>

#include <cassert>

UniformBuffer::UniformBuffer(GLuint index, GLint count)
{
	glm::mat4 type{}; // do I need other types here? If so: could be a in argument
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, count * sizeof(type), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, index, m_id, 0, count * sizeof(type)); // always binds the whole buffer
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void UniformBuffer::bindUniformBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
}

void UniformBuffer::unbindUniformBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::addUniformBufferSubData(const UniformBuffer& ub, BufferSubDataLayout& layout)
{	
	ub.bindUniformBuffer();
	const auto& bufferSubData{ layout.getBufferSubData() };
	GLintptr totalOffset{ 0 };
	for (GLuint i{ 0 }; i < bufferSubData.size(); i++) {
		const auto& bufferSubDataElement{ bufferSubData[i] };
		assert(bufferSubDataElement.m_target == GL_UNIFORM_BUFFER && "Wrong buffer binding target (must be GL_UNIFORM_BUFFER)");
		glBufferSubData(bufferSubDataElement.m_target, totalOffset, bufferSubDataElement.m_size, &bufferSubDataElement.m_data);
		totalOffset += bufferSubDataElement.m_size;
	}
}