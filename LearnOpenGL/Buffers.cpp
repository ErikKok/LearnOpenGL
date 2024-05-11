#include "Buffers.h"

FrameBuffer::FrameBuffer()
{
	glCreateFramebuffers(1, &m_id);
	//glCheckFramebufferStatus
	//glDeleteFramebuffers(1, &m_id);
}