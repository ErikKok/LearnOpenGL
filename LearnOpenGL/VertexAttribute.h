#pragma once

#include <glad/glad.h>

#include <cassert>
#include <vector>

struct VertexAttribute
{
	GLenum type{};
	GLint size{};
	GLboolean normalized{};
	uintptr_t offset{}; // index[0] will be reset to 0 by addVertexAttributeLayout
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

	inline void setVertexStride(GLsizei strideNew)
	{
		m_Stride = strideNew;
	}

	inline void setVertexOffset(int vertexAttributeIndex, uintptr_t offsetNew)
	{
		auto& vertexAttribute{ this->getVertexAttributes()};
		vertexAttribute[vertexAttributeIndex].offset = offsetNew;
	}

private:
	std::vector<VertexAttribute> m_Attributes;
	GLsizei m_Stride{};
};