#pragma once

#include "ShaderStorageBuffer.h"
#include "Global.h"

#include <print>

ShaderStorageBuffer::ShaderStorageBuffer(int bindingPoint, const auto& data)
	: m_bindingPoint{ bindingPoint }
{
	glCreateBuffers(1, &m_id);
	
	m_dataStore.setBufferId(m_id);
	m_dataStore.addBufferSubData(data);

	std::println("CREATE ShaderStorageBuffer id: {}", m_id);
}

ShaderStorageBuffer::ShaderStorageBuffer(int bindingPoint)
	: m_bindingPoint{ bindingPoint }
{
	glCreateBuffers(1, &m_id);
	Global::glCheckError();
	std::println("CREATE ShaderStorageBuffer id: {}", m_id);
}

ShaderStorageBuffer::ShaderStorageBuffer(int bindingPoint, int arrayCount)
	: m_arrayCount{ arrayCount }
	, m_bindingPoint{ bindingPoint }
{
	m_vector.resize(m_arrayCount);
	
	glCreateBuffers(1, &m_id);
	glNamedBufferStorage(m_id, sizeof(glm::mat4) * m_vector.size(), (const void*)m_vector.data(), GL_DYNAMIC_STORAGE_BIT);
	Global::glCheckError();
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

void ShaderStorageBuffer::unbind() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, 0);
	Global::glCheckError();
}

void ShaderStorageBuffer::upload() const
{
	glNamedBufferSubData(m_id, 0, sizeof(glm::mat4) * m_vector.size(), (const void*)m_vector.data());
	Global::glCheckError();
}

void ShaderStorageBuffer::update(const glm::mat4& vector, int i)
{
	m_vector[i] = vector;
}

void ShaderStorageBuffer::uploadAndBind() const
{
	upload();
	bind();
}

void ShaderStorageBuffer::updateAndUpload(const glm::mat4& vector, int i)
{
	update(vector, i);
	upload();
}

void ShaderStorageBuffer::updateAndUploadAndBind(const glm::mat4& vector, int i)
{
	update(vector, i);
	upload();
	bind();
}

/////

template<typename T>
void ShaderStorageBuffer::addBufferSubData(const T& data)
{
	m_dataStore.addBufferSubData(data);
}

template<typename T>
void ShaderStorageBuffer::addBufferSubData(const std::vector<T>& data)
{
	m_dataStore.addBufferSubData(data);
}

void ShaderStorageBuffer::createAndInitializeImmutableDataStore()
{
	m_dataStore.createAndInitializeImmutableDataStore();
}

void ShaderStorageBuffer::updateSubset(const auto& data, GLintptr elementIndex) const
{
	m_dataStore.updateSubset(data, elementIndex);
}

void ShaderStorageBuffer::updateAndUploadSubset(const auto& data, GLintptr elementIndex) const
{
	m_dataStore.updateAndUploadSubset(data, elementIndex);
}

void ShaderStorageBuffer::uploadFully(const auto& data) const
{
	m_dataStore.uploadFully(data);
}