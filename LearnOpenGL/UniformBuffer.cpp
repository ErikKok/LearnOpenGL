#pragma once
#include "UniformBuffer.h"
#include "Global.h"

#include <glad/glad.h>

#include <cassert>
#include <print>

UniformBuffer::UniformBuffer(int bindingPoint)
	: m_bindingPoint{ bindingPoint }
{
	glCreateBuffers(1, &m_id);
	Global::glCheckError();
	std::println("CREATE UniformBuffer id: {}", m_id);
}

//UniformBuffer::UniformBuffer(GLuint size, GLuint bindingPoint)
//{
//	glCreateBuffers(1, &m_id);
//	glNamedBufferStorage(m_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
//	// Bind the WHOLE UniformBuffer to the binding point defined in the shader
//	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_id);
//	// Bind only a specific range
//	//glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_id, offset, size2);
//	
//	Global::glCheckError();
//	std::println("CREATE UniformBuffer id: {}", m_id);
//}

UniformBuffer::~UniformBuffer()
{
	std::println("DELETE UniformBuffer id: {}", m_id);
	glDeleteBuffers(1, &m_id);
	Global::glCheckError();
}

void UniformBuffer::bindUniformBuffer() const
{
	std::println("BIND UniformBuffer id: {}", m_id);
	//glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_id);
	Global::glCheckError();
}

void UniformBuffer::unbindUniformBuffer() const
{
	std::println("UNBIND UniformBuffer id: {}", m_id);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, 0);
	Global::glCheckError();
}

// OLD non-DSA
//void UniformBuffer::addUniformBufferSubData(BufferSubDataLayout& layout) const
//{	
//	//std::println("ADD UniformBufferSubData id: {}", m_id);
//
//	assert(sizeof(layout.getBufferSubData()) != 0 && "WARNING: addUniformBufferSubData(): BufferSubDataLayout is empty!");
//	
//	GLint returnData;
//	glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &returnData);
//	if (returnData != static_cast<GLint>(this->getId())) {
//		std::println("WARNING: addUniformBufferSubData(): wrong UniformBuffer was bound -> corrected");
//		this->bindUniformBuffer();
//	}
//
//	const auto& bufferSubData{ layout.getBufferSubData() };
//	GLintptr totalOffset{ 0 };
//	for (GLuint i{ 0 }; i < bufferSubData.size(); i++) {
//		const auto& bufferSubDataElement{ bufferSubData[i] };
//		assert(bufferSubDataElement.m_target == GL_UNIFORM_BUFFER && "Wrong buffer binding target (must be GL_UNIFORM_BUFFER)");
//		glBufferSubData(bufferSubDataElement.m_target, totalOffset, bufferSubDataElement.m_size, &bufferSubDataElement.m_data);
//		totalOffset += bufferSubDataElement.m_size;
//	}
//	Global::glCheckError();
//}

// DSA
//void UniformBuffer::addUniformBufferSubData(const BufferSubDataLayout& layout) const
//{
//	//std::println("ADD UniformBufferSubData id: {}", m_id);
//
//	assert(sizeof(layout.getBufferSubData()) != 0 && "WARNING: addUniformBufferSubData(): BufferSubDataLayout is empty!");
//
//	const auto& bufferSubData{ layout.getBufferSubData() };
//	GLintptr totalOffset{ 0 };
//	for (GLuint i{ 0 }; i < bufferSubData.size(); i++) {
//		const auto& bufferSubDataElement{ bufferSubData[i] };
//		glNamedBufferSubData(m_id, totalOffset, bufferSubDataElement.m_size, &bufferSubDataElement.m_data);
//		totalOffset += bufferSubDataElement.m_size;
//	}
//
//	Global::glCheckError();
//}