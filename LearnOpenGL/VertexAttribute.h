#pragma once

#include <glad/glad.h>

#include <cassert>
#include <vector>

class VertexAttribute
{
	friend class VertexAttributeLayout;
	friend class VertexArray;

private:
	VertexAttribute(GLenum type, GLint count, GLboolean normalized, uintptr_t offset);

	GLenum m_type{};
	GLint m_count{}; // number of components/types of this attribute (Caution! named size @ docs.gl)
	GLboolean m_normalized{};
	uintptr_t m_offset{}; // size in bytes of this attribute
};

class VertexAttributeLayout {
public:
	VertexAttributeLayout(){};

	template<typename T>
	void pushVertexAttributeLayout(GLint count);
	template<>
	void pushVertexAttributeLayout<float>(GLint count);
	template<>
	void pushVertexAttributeLayout<int>(GLint count);
	template<>
	void pushVertexAttributeLayout<unsigned int>(GLint count);
	template<>
	void pushVertexAttributeLayout<unsigned char>(GLint count);

	inline const std::vector<VertexAttribute>& getVertexAttributes() const { return m_vertexAttributes;	}
	inline const unsigned int getStride() const {	return m_stride; }
	// Sets a custom Stride (the same for all the vertex attributes in the array)
	inline void setVertexStride(GLsizei strideNew) { m_stride = strideNew; }
	// Specifies a custom offset of the vertexAttribute
	// Usage example 1:	if there is x bytes unused between attribute y and attribute y+1
	//					use index y+1, offsetNew x
	// Usage example 2:	if attribute y+1 reuses the last x bytes of attribute y
	//					use index y+1, offsetNew -x
	inline void setVertexAttributeOffset(int vertexAttributeIndex, int offsetNew);

private:
	std::vector<VertexAttribute> m_vertexAttributes{};
	GLsizei m_stride{};
};