#pragma once

#include <glad/glad.h>

#include <cassert>
#include <vector>

class VertexAttribute
{
	friend class VertexAttributeLayout;
	friend class VertexArray;

private:
	VertexAttribute(GLenum type, GLint count, GLboolean normalized, uintptr_t offset)
		: m_type{ type }
		, m_count{ count } 
		, m_normalized{ normalized }
		, m_offset{ offset }
	{}

	GLenum m_type{};
	GLint m_count{}; // number of components/types of this attribute (Caution! named size @ docs.gl)
	GLboolean m_normalized{};
	uintptr_t m_offset{}; // size in bytes of this attribute
};

class VertexAttributeLayout {
public:
	VertexAttributeLayout() {};

	template<typename T>
	void pushVertexAttributeLayout(GLint count)
	{
		static_assert(false);
	}

	template<>
	void pushVertexAttributeLayout<float>(GLint count)
	{
		m_vertexAttributes.push_back({ GL_FLOAT, count, GL_FALSE, count * sizeof(GL_FLOAT) });
		m_stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void pushVertexAttributeLayout<int>(GLint count)
	{
		m_vertexAttributes.push_back({ GL_INT, count, GL_FALSE, count * sizeof(GL_INT) });
		m_stride += count * sizeof(GL_INT);
	}

	template<>
	void pushVertexAttributeLayout<unsigned int>(GLint count)
	{
		m_vertexAttributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, count * sizeof(GL_UNSIGNED_INT) });
		m_stride += count * sizeof(GL_UNSIGNED_INT);
	}

	template<>
	void pushVertexAttributeLayout<unsigned char>(GLint count)
	{
		m_vertexAttributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, count * sizeof(GL_UNSIGNED_BYTE) });
		m_stride += count * sizeof(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexAttribute>& getVertexAttributes()
	{
		return m_vertexAttributes;
	}

	inline unsigned int getStride() const
	{
		return m_stride;
	}

	// Sets a custom Stride (the same for all the vertex attributes in the array)
	inline void setVertexStride(GLsizei strideNew)
	{
		m_stride = strideNew;
	}

	// Specifies a custom offset of the vertexAttribute
	// Usage example 1:	if there is x bytes unused between attribute y and attribute y+1
	//					use index y+1, offsetNew x
	// Usage example 2:	if attribute y+1 reuses the last x bytes of attribute y
	//					use index y+1, offsetNew -x
	inline void setVertexAttributeOffset(int vertexAttributeIndex, int offsetNew)
	{
		assert(vertexAttributeIndex - 1u >= 0 && "You can not change vertexAttributeIndex #0");
		assert(static_cast<int>(m_vertexAttributes[vertexAttributeIndex].m_offset) + offsetNew >= 0 && "You cannot change offset to a negative value");

		m_vertexAttributes[(vertexAttributeIndex - 1u)].m_offset += offsetNew;
		m_stride += offsetNew;
	}

private:
	std::vector<VertexAttribute> m_vertexAttributes{};
	GLsizei m_stride{};
};