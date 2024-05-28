#pragma once
#include "VertexArray.h"

#include "Global.h"

#include <print>

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_id);
	Global::glCheckError();
	//std::println("CREATE VertexArray id: {}", m_id);
}

VertexArray::~VertexArray()
{
	std::println("DELETE VertexArray id: {}", m_id);
	glDeleteVertexArrays(1, &m_id); 
	Global::glCheckError();
}

void VertexArray::bindVertexArray() const
{
	glBindVertexArray(m_id);
	Global::glCheckError();
	//std::println("BIND VertexArray id: {}", m_id);
}

void VertexArray::unbindVertexArray() const
{
	std::println("UNBIND VertexArray id: {}", m_id);
	glBindVertexArray(0);
	Global::glCheckError();
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vbo, const VertexAttributeLayout& layout) const
{
	glVertexArrayVertexBuffer(m_id, 0, vbo.getId(), 0, layout.getStride());

	this->addVertexAttributeLayoutStep2(layout);
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vbo, const VertexAttributeLayout& layout, const ElementBuffer& ebo) const
{
	glVertexArrayVertexBuffer(m_id, 0, vbo.getId(), 0, layout.getStride());
	glVertexArrayElementBuffer(m_id, ebo.getId());

	this->addVertexAttributeLayoutStep2(layout);
}

void VertexArray::addVertexAttributeLayoutStep2(const VertexAttributeLayout& layout) const
{
	assert(sizeof(layout.getVertexAttributes()) != 0 && "ERROR: addVertexAttributeLayout(): VertexAttributeLayout is empty!");
	
	const auto& vertexAttributes{ layout.getVertexAttributes() };
	GLuint totalOffset{ 0 };
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glEnableVertexArrayAttrib(m_id, i);
		glVertexArrayAttribFormat(m_id, i, vertexAttribute.m_count, vertexAttribute.m_type, vertexAttribute.m_normalized, totalOffset);
		glVertexArrayAttribBinding(m_id, i, 0);
		totalOffset += vertexAttribute.m_offset;
	}

	Global::glCheckError();
	//std::println("ADD VertexAttributeLayoutStep2 id: {}", m_id);
}