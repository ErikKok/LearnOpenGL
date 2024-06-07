#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <any>
#include <vector>

// std430 packing layout
// only to be used with shader storage blocks, not uniform blocks
// https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout

class BufferDataStore {
public:
	BufferDataStore(const GLuint bufferId, const auto& data)
		: m_bufferId{ bufferId }
	{
		addBufferSubData(data);
	};

	// Disabled this function to control what types are accepted.
    // https://ktstephano.github.io/rendering/opengl/ssbos
	//template<typename T>
	//void pushUniformBufferSubData(const T& data)
	//{
	//	m_bufferSubData.push_back(data);
	//	Global::glCheckError();
	//}

	#pragma warning( suppress : 4100 )
	void addBufferSubData(const auto& data)
	{
		assert(false && "Data type not supported!");
	}

	void addBufferSubData(const glm::vec4& data)
	{	
		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
		m_elementSize = sizeof(data);
		m_data.emplace_back(data);
		Global::glCheckError();
	}

	void addBufferSubData(const glm::mat4& data)
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
		m_elementSize = sizeof(data);
		m_data.emplace_back(data);
		Global::glCheckError();
	}
	
	void addBufferSubData(const std::vector<glm::vec4>& data) // TODO test
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data[0]) && "Data has different size then existing data");
		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data.emplace_back(data[i]);
		}

		Global::glCheckError();
	}

	void addBufferSubData(const std::vector<glm::mat4>& data) // TODO test
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data[0]) && "Data has different size then existing data");
		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data.emplace_back(data[i]);
		}
		Global::glCheckError();
	}

	void addBufferSubData(const std::vector<testSSBO> data) // TODO test
	{
		assert(m_elementSize == 0 || m_elementSize == sizeof(data[0]) && "Data has different size then existing data");
		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data.emplace_back(data[i]);
		}
		Global::glCheckError();
	}

	void createAndInitializeImmutableDataStore()
	{
		assert(sizeof(m_data) != 0 && "WARNING: uploadBufferSubData(): BufferSubDataLayout is empty!");

		glNamedBufferStorage(m_bufferId, m_elementSize * static_cast<GLsizeiptr>(m_data.size()), nullptr, GL_DYNAMIC_STORAGE_BIT);

		// Upload data
		GLintptr offset{ 0 };
		for (auto i{ 0 }; i < std::ssize(m_data); i++) {
			glNamedBufferSubData(m_bufferId, offset, m_elementSize, &m_data[i]);
			offset += m_elementSize;
		}

		Global::glCheckError();
		//std::println("CREATE/UPLOAD BufferSubData id: {}", m_id);
	}

	void updateSubset(const auto& data, GLintptr elementIndex = 0) const
	{
		// TODO is this safe enough?

		assert(m_elementSize == sizeof(data) && "Data has different size then existing data"); // not a complete check; different types could have the same size
		assert(elementIndex <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");

		glNamedBufferSubData(m_bufferId, elementIndex * m_elementSize, m_elementSize, &data);

		Global::glCheckError();
	}

private:
	GLuint m_bufferId; // id of associated buffer
	std::vector<std::any> m_data;
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