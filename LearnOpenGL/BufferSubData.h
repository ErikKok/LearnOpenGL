#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

// TODO
// Meerdere types accepteren als BufferSubData (ipv alleen mat4)
// BufferSubData als template lukt wel, echter moet friend class BufferSubDataLayout
// dan ook ge-templated worden, en dat is lastig
// daarnaast moet ook addUniformBufferSubData de verschillende layout-types aankunnen
// die elk hun eigen size/offset hebben
// je zou 1 array moet hebben, met daarin alle layout-types (al dan niet als pointer of ref)
// die je vervolgens af kan gaan in addUniformBufferSubData
// of in addUniformBufferSubData auto als in-parameter
// en dan een switch obv type met de goede functie...

class BufferSubData
{
	friend class BufferSubDataLayout;
	friend class UniformBuffer;

private:
	BufferSubData(const glm::mat4& data)
		: m_data{ data }
	{}

	GLenum m_target{ GL_UNIFORM_BUFFER };
	GLsizeiptr m_size{ sizeof(glm::mat4) };
	const glm::mat4 m_data{};
};

class BufferSubDataLayout { // TODO declaration of this type needs to be in renderloop? why?
public:
	BufferSubDataLayout(){};

	void pushUniformBufferSubData(const glm::mat4& data)
	{
		m_bufferSubData.push_back(data);
		Global::glCheckError();
	}

	inline const std::vector<BufferSubData>& getBufferSubData() const { return m_bufferSubData;	}

private:
	std::vector<BufferSubData> m_bufferSubData;
};