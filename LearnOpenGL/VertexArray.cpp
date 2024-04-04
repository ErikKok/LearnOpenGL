#pragma once
#include "VertexArray.h"

#include "Global.h"
#include "VertexBuffer.h"

#include <print>

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Id);
	glBindVertexArray(m_Id);
	Global::glCheckError();
	std::println("CREATE VertexArray id: {}", m_Id);
}

VertexArray::~VertexArray()
{
	std::println("DELETE VertexArray id: {}", m_Id);
	glDeleteVertexArrays(1, &m_Id); 
	Global::glCheckError();
}

void VertexArray::bindVertexArray() const
{
	//std::println("BIND VertexArray id: {}", m_Id);
	glBindVertexArray(m_Id);
	Global::glCheckError();
}

void VertexArray::unbindVertexArray() const
{
	std::println("UNBIND VertexArray id: {}", m_Id);
	glBindVertexArray(0);
	Global::glCheckError();
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vb, VertexAttributeLayout& layout)
{
	std::println("ADD VertexAttributeLayout id: {}", m_Id);
	
	assert(sizeof(layout.getVertexAttributes()) != 0 && "WARNING: addVertexAttributeLayout(): VertexAttributeLayout is empty!");

	//glBindVertexArray(this->m_Id);
	GLint returnData;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
	assert(returnData == static_cast<GLint>(m_Id) && "WARNING: addVertexAttributeLayout(): Currently bound VertexArray is not equal to this->VertexArray");

	// TODO is het ok als verkeerde is gebound of moet dat altijd al goed zijn hier?
	// vb.bindVertexBuffer();
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
	assert(returnData == static_cast<GLint>(vb.getId()) && "WARNING: addVertexAttributeLayout(): wrong VertexBuffer was bound");

	const auto& vertexAttributes{ layout.getVertexAttributes() };
	uintptr_t totalOffset{0};
	for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
		const auto& vertexAttribute{ vertexAttributes[i] };
		glVertexAttribPointer(i, vertexAttribute.m_Count, vertexAttribute.m_Type, vertexAttribute.m_Normalized, layout.getStride(), (const void*)totalOffset);
		glEnableVertexAttribArray(i);
		totalOffset += vertexAttribute.m_Offset;
	}
	Global::glCheckError();
}