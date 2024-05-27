#pragma once
#include "VertexArray.h"

#include "Global.h"
//#include "VertexBuffer.h"

#include <print>

#include <glad/glad.h>

VertexArray::VertexArray()
{
	//glGenVertexArrays(1, &m_id);
	//glBindVertexArray(m_id);

	glCreateVertexArrays(1, &m_id);
	//glBindVertexArray(m_id); // TODO weg
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

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vbo, const VertexAttributeLayout& layout)
{
	//std::println("ADD VertexAttributeLayout id: {}", m_id);

	assert(sizeof(layout.getVertexAttributes()) != 0 && "ERROR: addVertexAttributeLayout(): VertexAttributeLayout is empty!");

	//GLint returnData;
	//glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
	//assert(returnData == static_cast<GLint>(m_id) && "ERROR: addVertexAttributeLayout(): Currently bound VertexArray is not equal to this->VertexArray");

	//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
	//if (returnData != static_cast<GLint>(vbo.getId())) {
	//	std::println("WARNING: addVertexAttributeLayout(): wrong VertexBuffer was bound -> corrected");
	//	vbo.bindVertexBuffer();
	//}

	//const auto& vertexAttributes{ layout.getVertexAttributes() };
	//uintptr_t totalOffset{0};
	//for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
	//	const auto& vertexAttribute{ vertexAttributes[i] };
	//	glVertexAttribPointer(i, vertexAttribute.m_count, vertexAttribute.m_type, vertexAttribute.m_normalized, layout.getStride(), (const void*)totalOffset);
	//	glEnableVertexAttribArray(i);
	//	totalOffset += vertexAttribute.m_offset;
	//}
	//Global::glCheckError();

	//glBindVertexBuffer(0, vbo.getId(), 0, sizeof(layout));

	glVertexArrayVertexBuffer(m_id, 0, vbo.getId(), 0, layout.getStride());
	//glVertexArrayElementBuffer(m_id, ebo.getId());

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
}

void VertexArray::addVertexAttributeLayout(const VertexBuffer& vbo, const ElementBuffer& ebo, const VertexAttributeLayout& layout)
{
	//std::println("ADD VertexAttributeLayout id: {}", m_id);
	
	assert(sizeof(layout.getVertexAttributes()) != 0 && "ERROR: addVertexAttributeLayout(): VertexAttributeLayout is empty!");

	//GLint returnData;
	//glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &returnData);
	//assert(returnData == static_cast<GLint>(m_id) && "ERROR: addVertexAttributeLayout(): Currently bound VertexArray is not equal to this->VertexArray");

	//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &returnData);
	//if (returnData != static_cast<GLint>(vbo.getId())) {
	//	std::println("WARNING: addVertexAttributeLayout(): wrong VertexBuffer was bound -> corrected");
	//	vbo.bindVertexBuffer();
	//}

	//const auto& vertexAttributes{ layout.getVertexAttributes() };
	//uintptr_t totalOffset{0};
	//for (GLuint i{ 0 }; i < vertexAttributes.size(); i++) {
	//	const auto& vertexAttribute{ vertexAttributes[i] };
	//	glVertexAttribPointer(i, vertexAttribute.m_count, vertexAttribute.m_type, vertexAttribute.m_normalized, layout.getStride(), (const void*)totalOffset);
	//	glEnableVertexAttribArray(i);
	//	totalOffset += vertexAttribute.m_offset;
	//}
	//Global::glCheckError();

	//glBindVertexBuffer(0, vbo.getId(), 0, sizeof(layout));

	glVertexArrayVertexBuffer(m_id, 0, vbo.getId(), 0, layout.getStride());
	glVertexArrayElementBuffer(m_id, ebo.getId());

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
}

// glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, pos));