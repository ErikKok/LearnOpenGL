#pragma once

#include "Global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

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
		m_BufferSubData.push_back({ data });
		Global::glCheckError();
	}

	inline std::vector<BufferSubData>& getBufferSubData()
	{
		return m_BufferSubData;
	}

private:
	std::vector<BufferSubData> m_BufferSubData;
};