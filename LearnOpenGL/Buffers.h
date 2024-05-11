#pragma once

#include <glad/glad.h>

class FrameBuffer {
public:
	FrameBuffer();

	const unsigned int getId() const { return m_id; };

private:
	GLuint m_id{};
};

// class GL_SHADER_STORAGE_BUFFER