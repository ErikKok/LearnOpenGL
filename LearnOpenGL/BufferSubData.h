#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <any>
#include <vector>

// Let op verschil std430 en std140

// https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout

// Among the most important is the fact that arrays of types are not necessarily tightly packed.An array of floats in such a block will not be the equivalent to an array of floats in C / C++.The array stride(the bytes between array elements) is always rounded up to the size of a vec4(ie: 16 - bytes).So arrays will only match their C / C++ definitions if the type is a multiple of 16 bytes

class BufferSubData
{
	friend class BufferSubDataLayout;
	friend class UniformBuffer; // TODO nodig?

private:
	template<typename T>
	BufferSubData(const T& data, size_t size)
		: m_data{ data }
		, m_size{ static_cast<GLsizeiptr>(size) }
	{}

	std::any m_data{};
	GLsizeiptr m_size{};
};

class BufferSubDataLayout {
public:
	BufferSubDataLayout(const GLuint bufferId, const auto& data)
		: m_bufferId{ bufferId }
	{
		addBufferSubData(data);
	};

	//template<typename T>
	//void pushUniformBufferSubData(const T& data)
	//{
	//	m_bufferSubData.push_back(data);
	//	Global::glCheckError();
	//  // Disabled this function to control what types are accepted. Otherwise you need to specify sizes yourself:
	//	// A vec3 needs to be aligned to a 16 byte boundary. A vec4 is also a 16 byte boundary.
	//	// A mat3 needs to be aligned to a 48 byte boundary. A mat4 needs to be aligned to a 64 byte boundary.
	//	// To make it easier for yourself, avoid using vec3 and mat3 in shader storage buffers.
	// 	// **OFFSET** must be multiple of 16 so 4->16! float / int / bool
	//  // https://ktstephano.github.io/rendering/opengl/ssbos
	//}

	#pragma warning( suppress : 4100 )
	void addBufferSubData(const auto& data)
	{
		assert(false && "Data type not supported!");
	}

	void addBufferSubData(const glm::vec4& data)
	{	
		m_bufferSubData.push_back(BufferSubData(data, sizeof(data)));
		Global::glCheckError();
	}

	void addBufferSubData(const glm::mat4& data)
	{
		m_bufferSubData.push_back(BufferSubData(data, sizeof(data)));
		Global::glCheckError();
	}
	
	void addBufferSubData(const std::vector<glm::vec4>& data) // TODO test
	{
		m_bufferSubData.push_back(BufferSubData(data, sizeof(glm::vec4) * data.size()));
		Global::glCheckError();
	}

	void addBufferSubData(const std::vector<glm::mat4>& data) // TODO test
	{
		m_bufferSubData.push_back(BufferSubData(data, sizeof(glm::mat4) * data.size()));
		Global::glCheckError();
	}

	void createBufferAndUploadData() const
	{
		//std::println("CREATE/UPLOAD BufferSubData id: {}", m_id);

		// TODO calculate totalSize kan ook eenmalig gebeuren na addBufferSubData ipv elke upload? nee, maakt niet uit, dit is toch eenmalig allemaal

		// Determine size of buffer to be created (the size is fixed for the rest of buffers life span)
		GLsizeiptr totalSize{ 0 }; // Specifies the size in bytes of the buffer object's new data store.
		for (int i{ 0 }; i < std::ssize(m_bufferSubData); i++) {
			totalSize += m_bufferSubData[i].m_size;
		}
		glNamedBufferStorage(m_bufferId, totalSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

		// Upload data
		assert(sizeof(m_bufferSubData) != 0 && "WARNING: uploadBufferSubData(): BufferSubDataLayout is empty!");

		const auto& bufferSubData{ m_bufferSubData };
		GLintptr totalOffset{ 0 };
		for (GLuint i{ 0 }; i < bufferSubData.size(); i++) {
			const auto& bufferSubDataElement{ bufferSubData[i] };
			glNamedBufferSubData(m_bufferId, totalOffset, bufferSubDataElement.m_size, &bufferSubDataElement.m_data);
			totalOffset += bufferSubDataElement.m_size;
		}

		Global::glCheckError();
	}

	// eventueel functie die een gedeelte van de buffer kan verversen

private:
	GLuint m_bufferId; // id of associated buffer
	std::vector<BufferSubData> m_bufferSubData;
};