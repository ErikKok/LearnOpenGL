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
		: m_Data{ data }

	{}
	GLenum m_Target{ GL_UNIFORM_BUFFER };
	GLsizeiptr m_Size{ sizeof(glm::mat4) };
	const glm::mat4& m_Data{};
};

class BufferSubDataLayout {
public:
	BufferSubDataLayout() {};

	void pushUniformBufferSubData(const glm::mat4& data)
	{
		m_BufferSubData.push_back(data);
		Global::glCheckError();
	}

	inline std::vector<BufferSubData>& getBufferSubData()
	{
		return m_BufferSubData;
	}

private:
	std::vector<BufferSubData> m_BufferSubData;
};