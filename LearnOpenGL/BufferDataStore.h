#pragma once

#include "Buffers.h"
#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <variant>
#include <vector>

// std430 packing layout
// only to be used with shader storage blocks, not uniform blocks
// https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout
// https://ktstephano.github.io/rendering/opengl/ssbos

class BufferDataStore {
	friend class ShaderStorageBuffer;

// Everything is to be used through bufferobject
private:
	//BufferDataStore() = default;

	BufferDataStore(int elementCount, GLsizeiptr elementSize)
		: m_elementSize{ elementSize }
	{
		m_data.resize(elementCount);
	};

	void setBufferId(GLuint ssboId) { m_ssboId = ssboId; };

	void createImmutableDataStore() const
	{
		assert(m_ssboId != 0 && "m_ssboId not set");
		glNamedBufferStorage(m_ssboId, m_data.size() * m_elementSize, m_data.data(), GL_DYNAMIC_STORAGE_BIT);
		Global::glCheckError();
		// Size of the buffer is fixed for the rest of it's life span from here on
	};

	// Fill or replace the complete buffer (just one 'element')
	template<typename T>
	void addBufferSubData(const T& data)
	{
		assert(sizeof(m_data) != 0 && "WARNING: data is empty!");
		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
		m_elementSize = sizeof(data);
		m_data = data;
	}

	// Fill or replace the complete buffer from a vector
	template<typename T>
	void addBufferSubData(const std::vector<T>& data)
	{
		assert(sizeof(m_data) != 0 && "WARNING: data is empty!");
		assert(m_elementSize == 0 || m_elementSize == sizeof(data[0]) && "Data has different size then existing data");
		assert(m_data.size() == data.size() && "Data has different elementcount then existing data");

		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data[i] = data[i];
		}
	}

	// Replace just 1 element of the buffer
	void updateSubset(const auto& data, GLintptr elementIndex = 0)
	{
		// TODO is this safe enough?
		assert(sizeof(m_data) != 0 && "WARNING: data is empty!");
		assert(m_elementSize == sizeof(data) && "Data has different size then existing data"); // TODO not a complete check; different types could have the same size
		assert(elementIndex <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");

		m_data[elementIndex] = data;
	}

	// Replace just 1 element of the buffer + upload just that element
	void updateAndUploadSubset(const auto& data, GLintptr elementIndex = 0)
	{
		updateSubset(data, elementIndex);

		glNamedBufferSubData(m_ssboId, elementIndex * m_elementSize, m_elementSize, &data);

		Global::glCheckError();
	}

	// Uuploads the complete buffer from cpu to gpu
	void uploadFully() const // also creates the buffer
	{
		GLintptr offset{ 0 };
		for (auto i{ 0 }; i < std::ssize(m_data); i++) {
			glNamedBufferSubData(m_ssboId, offset, m_elementSize, &m_data[i]);
			offset += m_elementSize;
		}

		Global::glCheckError();
	}

private:
	GLuint m_ssboId{ 0 }; // id of associated ssbo (only ssbo because of std430 packing layout)
	std::vector<std::variant<glm::vec4, glm::mat4>> m_data{};
	GLsizeiptr m_elementSize{ 0 };
};

// I tried to not use std::any

// std::any en dan een pointer vragen werkt niet
// std::any_cast<glm::vec4>(&m_bufferSubData[0].m_dataVector)); // moet het type vantevoren weten...

//template<typename T>
//BufferSubData(const std::vector<T>& data, size_t size)
//	: m_dataVector{ data }
//	, m_size{ static_cast<GLsizeiptr>(size) }
//{}

//std::vector<std::any> m_dataVector; // Je krijgt een vector in vector, probleem dat je .data() niet kunt gebruiken blijft

//GLintptr totalOffset{ 0 };
//for (auto i{ 0 }; i < std::ssize(m_bufferSubData); i++) {
//	if (m_bufferSubData[i].m_data.has_value())
//	glNamedBufferSubData(m_bufferId, totalOffset, m_bufferSubData[i].m_size, &m_bufferSubData[i].m_data);
//	else 
//		glNamedBufferSubData(m_bufferId, totalOffset, m_bufferSubData[i].m_size, m_bufferSubData[i].m_dataVector.data());
//	totalOffset += m_bufferSubData[i].m_size;
//}