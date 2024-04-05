#pragma once
#include "VertexArray.h"

#include "Global.h"
#include "VertexBuffer.h"

#include <print>

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);
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
	//std::println("BIND VertexArray id: {}", m_id);
	glBindVertexArray(m_id);
	Global::glCheckError();
}

void VertexArray::unbindVertexArray() const
{
	std::println("UNBIND VertexArray id: {}", m_id);
	glBindVertexArray(0);
	Global::glCheckError();
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
{
	//std::println("ADD VertexAttributeLayout id: {}", m_id);
	
	assert(sizeof(layout.getVertexAttributes()) != 0 && "WARNING: addVertexAttributeLayout(): VertexAttributeLayout is empty!");

	//glBindVertexArray(this->m_id);
	GLint returnData;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
	assert(returnData == static_cast<GLint>(m_id) && "WARNING: addVertexAttributeLayout(): Currently bound VertexArray is not equal to this->VertexArray");

	// TODO is het ok als verkeerde is gebound of moet dat altijd al goed zijn hier?
	// vb.bindVertexBuffer();
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
	assert(returnData == static_cast<GLint>(vb.getId()) && "WARNING: addVertexAttributeLayout(): wrong VertexBuffer was bound");

	const auto& vertexAttributes{ layout.getVertexAttributes() };
	uintptr_t totalOffset{0};
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glVertexAttribPointer(i, vertexAttribute.m_count, vertexAttribute.m_type, vertexAttribute.m_normalized, layout.getStride(), (const void*)totalOffset);
		glEnableVertexAttribArray(i);
		totalOffset += vertexAttribute.m_offset;
	}
	Global::glCheckError();
}