#pragma once

#include <glad/glad.h>

class FrameBuffer {
public:
	FrameBuffer();

private:
	GLuint m_id{};
};