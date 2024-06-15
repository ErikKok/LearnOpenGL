#pragma once

#include "BufferDataStore.h"

BufferDataStore::BufferDataStore(int elementCount, GLsizeiptr elementSize)
	: m_elementSize{ elementSize }
{
	m_data.resize(elementCount);
};

void BufferDataStore::createImmutableDataStore() const
{
	assert(m_ssboId != 0 && "m_ssboId not set");
	glNamedBufferStorage(m_ssboId, m_data.size() * m_elementSize, m_data.data(), GL_DYNAMIC_STORAGE_BIT);
	Global::glCheckError();
};

void BufferDataStore::uploadFully()
{
	GLintptr offset{ 0 };
	for (auto i{ 0 }; i < std::ssize(m_data); i++) {
		glNamedBufferSubData(m_ssboId, offset, m_elementSize, &m_data[i]);
		offset += m_elementSize;
	}

	Global::glCheckError();
	m_hasValidData = true;
}