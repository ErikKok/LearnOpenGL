#pragma once

#include "ShaderStorageBuffer.h"
#include "Global.h"

#include <print>

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

void ShaderStorageBuffer::update() const
{
	glNamedBufferSubData(m_id, 0, sizeof(glm::mat4) * m_vector.size(), (const void*)m_vector.data());
	Global::glCheckError();
}

void ShaderStorageBuffer::setVector(const glm::mat4& vector, int i)
{
	m_vector[i] = vector;
}

void ShaderStorageBuffer::updateAndBind() const
{
	this->update();
	this->bind();
}

void ShaderStorageBuffer::setVectorAndUpdateAndBind(const glm::mat4& vector, int i)
{
	this->setVector(vector, i);
	this->update();
	this->bind();
}