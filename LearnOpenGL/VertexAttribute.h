#pragma once

#include <glad/glad.h>

#include <vector>

class VertexAttribute
{
	friend class VertexAttributeLayout;
	friend class VertexArray;

private:
	VertexAttribute(GLenum type, GLint count, GLboolean normalized, uintptr_t offset)
		: m_Type{ type }
		, m_Count{ count } // number of components/types of this attribute (count * sizeof(type)
		, m_Normalized{ normalized }
		, m_Offset{ offset } // size in bytes of this attribute
	{}

	GLenum m_Type{};
	GLint m_Count{};
	GLboolean m_Normalized{};
	uintptr_t m_Offset{};
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
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE, count * sizeof(GL_FLOAT) });
		m_Stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void pushVertexAttributeLayout<unsigned int>(GLint count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, count * sizeof(GL_UNSIGNED_INT) });
		m_Stride += count * sizeof(GL_UNSIGNED_INT);
	}

	template<>
	void pushVertexAttributeLayout<unsigned char>(GLint count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, count * sizeof(GL_UNSIGNED_BYTE) });
		m_Stride += count * sizeof(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexAttribute>& getVertexAttributes()
	{
		return m_Attributes;
	}

	inline unsigned int getStride() const
	{
		return m_Stride;
	}

	// Sets a custom Stride (the same for all the vertex attributes in the array)
	inline void setVertexStride(GLsizei strideNew)
	{
		m_Stride = strideNew;
	}

	// Specifies a custom offset of the first component of the first generic vertex attribute in the array
	inline void setVertexAttributeOffset(int vertexAttributeIndex, uintptr_t offsetNew)
	{
		m_Attributes[vertexAttributeIndex].m_Offset = offsetNew;
	}

private:
	std::vector<VertexAttribute> m_Attributes;
	GLsizei m_Stride{};
};