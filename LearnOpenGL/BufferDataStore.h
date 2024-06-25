#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <concepts>
#include <typeinfo>
#include <variant>
#include <vector>

// std430 packing layout
// only to be used with shader storage blocks, not uniform blocks
// https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout
// https://ktstephano.github.io/rendering/opengl/ssbos

struct uberSSBO {
	alignas(16) glm::mat4 dirLightMVPMatrix[10]; // alignas(16) just to be sure, not 100% needed
	glm::mat4 flashLightMVPMatrix[10];
	glm::mat4 spotLightMVPMatrix[10];
	glm::mat4 normalMatrix[10];
	glm::mat4 modelViewMatrix[10];
	glm::mat4 MVPMatrix[10];
};

class BufferDataStore {
	friend class ShaderStorageBuffer;

// Everything is to be used through bufferobject
private:
	BufferDataStore() = delete;

	BufferDataStore(int elementCount, GLsizeiptr elementSize);

	void setBufferId(GLuint ssboId) { m_ssboId = ssboId; };
	void createImmutableDataStore() const; // Size of the buffer is fixed for the rest of BufferDataStore's life span from here on

	// Function Templates fully defined in this header file, see https://www.learncpp.com/cpp-tutorial/function-template-instantiation/

	// Replace just 1 element of the local buffer
	void updateSubset(const auto& data, GLintptr elementIndex, bool upload)
	{
		if (m_hasValidData) {
			assert(
				std::holds_alternative<glm::vec4>(m_data[elementIndex]) & (typeid(data).hash_code() == typeid(glm::vec4).hash_code()) ||
				std::holds_alternative<glm::mat4>(m_data[elementIndex]) & (typeid(data).hash_code() == typeid(glm::mat4).hash_code()) ||
				std::holds_alternative<uberSSBO>(m_data[elementIndex]) & (typeid(data).hash_code() == typeid(uberSSBO).hash_code()));
		}
		assert(sizeof(m_data) != 0 && "WARNING: m_data is not resized!");
		assert(elementIndex <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");

		m_data[elementIndex] = data;

		if (upload) {
			glNamedBufferSubData(m_ssboId, elementIndex * m_elementSize, m_elementSize, &data);

			Global::glCheckError();
			m_hasValidData = true;
		}
	}

	// Fill or replace the complete local buffer (from a single object)
	template<typename T>
	void updateFully(const T& data, bool upload)
	{
		if (m_hasValidData) {
			assert(
				std::holds_alternative<glm::vec4>(m_data[0]) & (typeid(data).hash_code() == typeid(glm::vec4).hash_code()) ||
				std::holds_alternative<glm::mat4>(m_data[0]) & (typeid(data).hash_code() == typeid(glm::mat4).hash_code()) ||
				std::holds_alternative<uberSSBO>(m_data[0]) & (typeid(data).hash_code() == typeid(uberSSBO).hash_code()) );
		}
		assert(sizeof(m_data) != 0 && "WARNING: m_data is not resized!");
		m_elementSize = sizeof(data);
		m_data[0] = data;

		if (upload)
			uploadFully();
	}

	// Fill or replace the complete local buffer from a vector
	template<typename T>
	void updateFully(const std::vector<T>& data, bool upload)
	{
		if (m_hasValidData) {
			assert(
				std::holds_alternative<glm::vec4>(m_data[0]) & (typeid(data[0]).hash_code() == typeid(glm::vec4).hash_code()) ||
				std::holds_alternative<glm::mat4>(m_data[0]) & (typeid(data[0]).hash_code() == typeid(glm::mat4).hash_code()) ||
				std::holds_alternative<uberSSBO>(m_data[0]) & (typeid(data[0]).hash_code() == typeid(uberSSBO).hash_code()) );
		}
		assert(sizeof(m_data) != 0 && "WARNING: m_data is not resized!");
		assert(m_data.size() == data.size() && "Data has different elementcount then existing data");

		m_elementSize = sizeof(data[0]);
		for (auto i{ 0 }; i < std::ssize(data); i++) {
			m_data[i] = data[i];
		}

		if (upload)
			uploadFully();
	}

	// Uploads all elements of the buffer until the specified ElementIndex to the GPU (untilElement itself is not included in the upload)
	// Defaults to upload element 0
	// If you want to upload a specific Subset, you could use updateSubset() with upload = true
	void uploadUntilSubset(int untilElementIndex = 1) {
		assert((untilElementIndex - 1) <= static_cast<GLintptr>(m_data.size()) - 1 && "ElementIndex out of range");
		
		GLintptr offset{ 0 };
		for (auto i{ 0 }; i < untilElementIndex; i++) {
			glNamedBufferSubData(m_ssboId, offset, m_elementSize, &m_data[i]);
			offset += m_elementSize;
		}

		Global::glCheckError();
		m_hasValidData = true;
	}

	void uploadFully()
	{
		GLintptr offset{ 0 };
		for (auto i{ 0 }; i < std::ssize(m_data); i++) {
			glNamedBufferSubData(m_ssboId, offset, m_elementSize, &m_data[i]);
			offset += m_elementSize;
		}

		Global::glCheckError();
		m_hasValidData = true;
	}

private:
	GLuint m_ssboId{ 0 }; // id of associated ssbo (only ssbo for now because of std430 packing layout)
	bool m_hasValidData{ false }; // if false m_data has only been resized by constructor
	std::vector<std::variant<glm::vec4, glm::mat4, uberSSBO>> m_data{};
	GLsizeiptr m_elementSize{ 0 };
};

// I tried to not use std::any/std::variant:

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


	//// Replace just 1 element of the buffer + upload to GPU
	//void updateAndUploadSubset(const auto& data, GLintptr elementIndex = 0)
	//{
	//	updateSubset(data, elementIndex);

	//	glNamedBufferSubData(m_ssboId, elementIndex * m_elementSize, m_elementSize, &data);

	//	Global::glCheckError();
	//	m_hasValidData = true;
	//}
    // In ShaderStorageBuffer.h:
	//void updateAndUploadSubset(const auto& data, GLintptr elementIndex = 0) { m_BufferDataStore.updateAndUploadSubset(data, elementIndex); };