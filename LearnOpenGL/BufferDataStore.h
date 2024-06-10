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

public:
	BufferDataStore() = default;

	BufferDataStore(const GLuint ssboId, const auto& data)
		: m_ssboId{ ssboId }
	{
		addBufferSubData(data);
	};

	void setBufferId(GLuint ssboId) { m_ssboId = ssboId; };

// functions are to be used through bufferobject

	template<typename T>
	void addBufferSubData(const T& data)
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
		m_elementSize = sizeof(data);
		m_data.emplace_back(data);
	}

	template<typename T>
	void addBufferSubData(const std::vector<T>& data)
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data[0]) && "Data has different size then existing data");
		
		//m_data.reserve(std::ssize(data)); reserve ipv resize ok?

		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data.emplace_back(data[i]);
		}
	}

	void createAndInitializeImmutableDataStore()
	{
		assert(sizeof(m_data) != 0 && "WARNING: uploadBufferSubData(): BufferSubDataLayout is empty!");
		assert(m_ssboId != 0 && "m_ssboId not set");

		glNamedBufferStorage(m_ssboId, m_elementSize * static_cast<GLsizeiptr>(m_data.size()), nullptr, GL_DYNAMIC_STORAGE_BIT);

		// Upload data
		GLintptr offset{ 0 };
		for (auto i{ 0 }; i < std::ssize(m_data); i++) {
			glNamedBufferSubData(m_ssboId, offset, m_elementSize, &m_data[i]);
			offset += m_elementSize;
		}

		Global::glCheckError();
		//std::println("CREATE/UPLOAD BufferSubData id: {}", m_id);
	}

	// Size of the buffer is fixed for the rest of it's life span from here on
	//////////////////////////////////////////////////////////////////////////

	void updateSubset(const auto& data, GLintptr elementIndex = 0)
	{
		// TODO is this safe enough?

		assert(m_elementSize == sizeof(data) && "Data has different size then existing data"); // TODO not a complete check; different types could have the same size
		assert(elementIndex <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");

		m_data[elementIndex] = data;
	}

	void updateAndUploadSubset(const auto& data, GLintptr elementIndex = 0)
	{
		updateSubset(data, elementIndex);

		glNamedBufferSubData(m_ssboId, elementIndex * m_elementSize, m_elementSize, &data);

		Global::glCheckError();
	}

	// DO I need this? -> void updateFully(const auto& data) const

	void uploadFully(const auto& data) const
	{
		glNamedBufferStorage(m_ssboId, m_elementSize * static_cast<GLsizeiptr>(m_data.size()), nullptr, GL_DYNAMIC_STORAGE_BIT);

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