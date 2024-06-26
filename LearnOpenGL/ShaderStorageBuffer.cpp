#pragma once

#include "ShaderStorageBuffer.h"

#include <print>

ShaderStorageBuffer::ShaderStorageBuffer(GLuint bindingPoint, int elementCount, GLsizeiptr elementSize, SSBO type)
	: m_ssboType { type }
	, m_bindingPoint{ bindingPoint }
	, m_BufferDataStore{ elementCount, elementSize }
{
	assert(type == SSBO::undefined || m_bindingPoint == std::to_underlying(type) && "BindingPoint and type are not equal");

	glCreateBuffers(1, &m_id);
	m_BufferDataStore.setBufferId(m_id);
	m_BufferDataStore.createImmutableDataStore();

	std::println("CREATE ShaderStorageBuffer id: {}", m_id);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	std::println("DELETE ShaderStorageBuffer id: {}", m_id);
	glDeleteBuffers(1, &m_id);
	Global::glCheckError();
}

void ShaderStorageBuffer::bind() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_id);
	Global::glCheckError();
}

void ShaderStorageBuffer::bindOverrideBindingPoint(GLuint BP) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BP, m_id);
	Global::glCheckError();
}

//void ShaderStorageBuffer::unbind() const
//{
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, 0);
//	Global::glCheckError();
//}