#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <vector>

class BufferSubData
{
	friend class BufferSubDataLayout;
	friend class UniformBuffer;

private:
	BufferSubData(const glm::mat4& data) // uintptr_t offset, 
//		, m_offset{ offset } // size in bytes of this attribute
//		: m_size{ size } // Specifies the size in bytes of the data store region being replaced
		: m_data{ data }

	{}
	GLenum m_target{ GL_UNIFORM_BUFFER };
	//GLintptr m_offset{};
	GLsizeiptr m_size{ sizeof(glm::mat4) };
	const glm::mat4& m_data{};
};

class BufferSubDataLayout {
public:
	BufferSubDataLayout() {};

	//template<typename T>
	//void pushUniformBufferSubData(const glm::mat4& data)
	//{
	//	static_assert(false);
	//}

//	template<>
	void pushUniformBufferSubData(const glm::mat4& data)
	{
		m_bufferSubData.push_back({ data });
		//m_stride += count * sizeof(GL_FLOAT);
	}

	//template<>
	//void pushUniformBufferSubData<unsigned int>(GLint count)
	//{
	//	m_bufferSubData.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, count * sizeof(GL_UNSIGNED_INT) });
	//	//m_stride += count * sizeof(GL_UNSIGNED_INT);
	//}

	//template<>
	//void pushUniformBufferSubData<unsigned char>(GLint count)
	//{
	//	m_bufferSubData.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, count * sizeof(GL_UNSIGNED_BYTE) });
	//	//m_stride += count * sizeof(GL_UNSIGNED_BYTE);
	//}

	inline std::vector<BufferSubData>& getBufferSubData()
	{
		return m_bufferSubData;
	}

	//inline unsigned int getStride() const
	//{
	//	return m_stride;
	//}

	//// Sets a custom Stride (the same for all the vertex attributes in the array)
	//inline void setVertexStride(GLsizei strideNew)
	//{
	//	m_stride = strideNew;
	//}

	// Specifies a custom offset of the first component of the first generic vertex attribute in the array
	//inline void setBufferSubDataLayoutOffset(int vertexAttributeIndex, uintptr_t offsetNew)
	//{
	//	m_bufferSubData[BufferSubData].m_offset = offsetNew;
	//}

private:
	std::vector<BufferSubData> m_bufferSubData;
	//GLsizei m_stride{};
};