//#pragma once
//#include "BufferDataStore.h"
//
////#include "Global.h"
////
////#include <glad/glad.h>
////#include <glm/glm.hpp>
////
////#include <any>
////#include <vector>
//
//
//
//	BufferDataStore::BufferDataStore(const GLuint bufferId, const auto& data)
//		: m_bufferId{ bufferId }
//	{
//		addBufferSubData(data);
//	}
//
//#pragma warning( suppress : 4100 )
//	void BufferDataStore::addBufferSubData(const auto& data)
//	{
//		assert(false && "Data type not supported!");
//	}
//
//	void BufferDataStore::addBufferSubData(const glm::vec4& data)
//	{
//		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
//		m_elementSize = sizeof(data);
//		m_data.emplace_back(data);
//		Global::glCheckError();
//	}
//
//	void BufferDataStore::addBufferSubData(const glm::mat4& data)
//	{
//		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
//		m_elementSize = sizeof(data[0]);
//		m_data.emplace_back(data);
//		Global::glCheckError();
//	}
//
//	void BufferDataStore::addBufferSubData(const std::vector<glm::vec4>& data) // TODO test
//	{
//		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
//		m_elementSize = sizeof(data[0]);
//		for (auto i{ 0 }; i < std::ssize(data); i++) {
//			m_data.emplace_back(data[i]);
//		}
//
//		Global::glCheckError();
//	}
//
//	void BufferDataStore::addBufferSubData(const std::vector<glm::mat4>& data) // TODO test
//	{
//		assert(m_elementSize == 0 || m_elementSize == sizeof(data) && "Data has different size then existing data");
//		m_elementSize = sizeof(data);
//		for (auto i{ 0 }; i < std::ssize(data); i++) {
//			m_data.emplace_back(data[i]);
//		}
//		Global::glCheckError();
//	}
//
//	void BufferDataStore::createAndInitializeImmutableDataStore()
//	{
//		assert(sizeof(m_data) != 0 && "WARNING: uploadBufferSubData(): BufferSubDataLayout is empty!");
//
//		glNamedBufferStorage(m_bufferId, m_elementSize * static_cast<GLsizeiptr>(m_data.size()), nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//		// Upload data
//		GLintptr offset{ 0 };
//		for (auto i{ 0 }; i < std::ssize(m_data); i++) {
//			glNamedBufferSubData(m_bufferId, offset, m_elementSize, &m_data[i]);
//			offset += m_elementSize;
//		}
//
//		Global::glCheckError();
//		//std::println("CREATE/UPLOAD BufferSubData id: {}", m_id);
//	}
//
//	void BufferDataStore::updateSubset(const auto& data, GLintptr elementIndex) const
//	{
//		// TODO is this safe enough?
//
//		assert(m_elementSize == sizeof(data) && "Data has different size then existing data"); // not a complete check; different types could have the same size
//		assert(elementIndex <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");
//
//		glNamedBufferSubData(m_bufferId, elementIndex * m_elementSize, m_elementSize, &data);
//
//		Global::glCheckError();
//	}