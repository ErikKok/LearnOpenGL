#pragma once

#include "VertexArray.h"

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_id);
	G::glCheckError();
	//std::println("CREATE VertexArray id: {}", m_id);
}

VertexArray::~VertexArray()
{
	std::println("DELETE VertexArray id: {}", m_id);
	glDeleteVertexArrays(1, &m_id); 
	G::glCheckError();
}

void VertexArray::bindVertexArray() const
{
	glBindVertexArray(m_id);
	G::glCheckError();
	//std::println("BIND VertexArray id: {}", m_id);
}

void VertexArray::finalizeVertexAttributeLayout(const VertexBuffer* vbo, const VertexAttributeLayout* layout) const
{
	assert(sizeof(layout->m_vertexAttributes) != 0 && "ERROR: addVertexAttributeLayout(): VertexAttributeLayout is empty!");
	
	// Bind vbo to vao, as layout is now final
	glVertexArrayVertexBuffer(m_id, 0, vbo->getId(), 0, layout->m_stride);

	GLuint totalOffset{ 0 };
	for (auto i{ 0 }; i < std::ssize(layout->m_vertexAttributes); i++) {
		glEnableVertexArrayAttrib(m_id, static_cast<GLuint>(i));
		glVertexArrayAttribFormat(m_id, static_cast<GLuint>(i), layout->m_vertexAttributes[i].m_count, layout->m_vertexAttributes[i].m_type, layout->m_vertexAttributes[i].m_normalized, totalOffset);
		glVertexArrayAttribBinding(m_id, static_cast<GLuint>(i), 0);
		totalOffset += layout->m_vertexAttributes[i].m_offset;
	}

	G::glCheckError();
	//std::println("ADD VertexAttributeLayoutStep2 id: {}", m_id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VertexAttribute::VertexAttribute(GLenum type, GLint count, GLboolean normalized, GLuint offset)
	: m_type{ type }
	, m_count{ count }
	, m_normalized{ normalized }
	, m_offset{ offset }
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void VertexAttributeLayout::pushVertexAttributeLayout(GLint count)
{
	static_assert(false); // should never be called directly, all types are covered in the overloads below
}

template<>
void VertexAttributeLayout::pushVertexAttributeLayout<float>(GLint count)
{
	m_vertexAttributes.push_back({ GL_FLOAT, count, GL_FALSE, static_cast<GLuint>(count * sizeof(GLfloat)) });
	m_stride += count * sizeof(GLfloat);
}

template<>
void VertexAttributeLayout::pushVertexAttributeLayout<int>(GLint count)
{
	m_vertexAttributes.push_back({ GL_INT, count, GL_FALSE, static_cast<GLuint>(count * sizeof(GLint)) });
	m_stride += count * sizeof(GLint);
}

template<>
void VertexAttributeLayout::pushVertexAttributeLayout<unsigned int>(GLint count)
{
	m_vertexAttributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, static_cast<GLuint>(count * sizeof(GLuint)) });
	m_stride += count * sizeof(GLuint);
}

template<>
void VertexAttributeLayout::pushVertexAttributeLayout<unsigned char>(GLint count)
{
	m_vertexAttributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE, static_cast<GLuint>(count * sizeof(GLuint)) });
	m_stride += count * sizeof(GLuint);
}

inline void VertexAttributeLayout::setVertexAttributeOffset(int vertexAttributeIndex, int offsetNew)
{
	assert(vertexAttributeIndex - 1u >= 0 && "You can not change vertexAttributeIndex #0");
	assert(static_cast<int>(m_vertexAttributes[vertexAttributeIndex].m_offset) + offsetNew >= 0 && "You cannot change offset to a negative value");

	m_vertexAttributes[(vertexAttributeIndex - 1u)].m_offset += offsetNew;
	m_stride += offsetNew;
}