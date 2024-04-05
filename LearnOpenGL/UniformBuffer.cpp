#pragma once
#include "UniformBuffer.h"
#include "Global.h"

#include <glad/glad.h>

#include <cassert>
#include <print>

UniformBuffer::UniformBuffer(size_t size, GLuint index)
{
	size = static_cast<GLuint>(size);
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, index, m_Id, 0, size); // always binds the whole buffer
	Global::glCheckError();
	std::println("CREATE UniformBuffer id: {}", m_Id);
}

UniformBuffer::~UniformBuffer()
{
	std::println("DELETE UniformBuffer id: {}", m_Id);
	glDeleteBuffers(1, &m_Id);
	Global::glCheckError();
}

void UniformBuffer::bindUniformBuffer() const
{
	std::println("BIND UniformBuffer id: {}", m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	Global::glCheckError();
}

void UniformBuffer::unbindUniformBuffer() const
{
	std::println("UNBIND UniformBuffer id: {}", m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	Global::glCheckError();
}

void UniformBuffer::addUniformBufferSubData(const UniformBuffer& ub, BufferSubDataLayout& layout)
{	
	//std::println("ADD UniformBufferSubData id: {}", m_Id);

	assert(sizeof(layout.getBufferSubData()) != 0 && "WARNING: addUniformBufferSubData(): BufferSubDataLayout is empty!");

	// TODO is het ok als verkeerde is gebound of moet dat altijd al goed zijn hier? -> NEEN dus if maken
	// ub.bindUniformBuffer();
	GLint returnData;
	glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &returnData);
	assert(returnData == static_cast<GLint>(ub.getId()) && "WARNING: addUniformBufferSubData(): wrong UniformBuffer was bound");

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