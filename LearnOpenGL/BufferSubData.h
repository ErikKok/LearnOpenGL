#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <any>
#include <vector>

// TODO
// Meerdere types accepteren als BufferSubData (ipv alleen mat4)
// BufferSubData als template lukt wel, echter moet friend class BufferSubDataLayout
// dan ook ge-templated worden, en dat is lastig
// daarnaast moet ook addUniformBufferSubData de verschillende layout-types aankunnen
// die elk hun eigen size/offset hebben
// je zou 1 array moet hebben, met daarin alle layout-types (al dan niet als pointer of ref)
// die je vervolgens af kan gaan in addUniformBufferSubData
// of in addUniformBufferSubData auto als in-parameter
// en dan een switch obv type met de goede functie...

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

	const std::any m_data{};
	GLsizeiptr m_size{};
	//GLenum m_target{ GL_UNIFORM_BUFFER }; // not needed for DSA
};

class BufferSubDataLayout { // TODO declaration of this type needs to be in renderloop? why?
public:
	BufferSubDataLayout(const GLuint bufferId)
		: m_bufferId{ bufferId }
	{};

	//template<typename T>
	//void pushUniformBufferSubData(const T& data)
	//{
	//	m_bufferSubData.push_back(data);
	//	Global::glCheckError();
	//  // Disabled this function to control what types are accepted. Otherwise you need to specify sizes yourself:
	//	// A vec3 needs to be aligned to a 16 byte boundary. A vec4 is also a 16 byte boundary.
	//	// A mat3 needs to be aligned to a 48 byte boundary. A mat4 needs to be aligned to a 64 byte boundary.
	//	// To make it easier for yourself, avoid using vec3 and mat3 in shader storage buffers.
	//  // https://ktstephano.github.io/rendering/opengl/ssbos
	//}

	/// (**OFFSET** must be multiple of 16 so 4->16) !!!!!!!! float / int / bool

	void pushUniformBufferSubData(const glm::vec4& data)
	{	
		m_bufferSubData.push_back(BufferSubData(data, sizeof(data)));
		Global::glCheckError();
	}

	void pushUniformBufferSubData(const glm::mat4& data)
	{
		m_bufferSubData.push_back(BufferSubData(data, sizeof(data)));
		Global::glCheckError();
	}

	void createBufferAndUploadData() const
	{
		//std::println("UPLOAD BufferSubData id: {}", m_id);

		// Determine size of buffer to be created (the size is fixed for the rest of buffers life span) TODO ja voor een ssbo, maar voor andere buffers?
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

private:
	GLuint m_bufferId; // id of associated buffer
	std::vector<BufferSubData> m_bufferSubData;
};