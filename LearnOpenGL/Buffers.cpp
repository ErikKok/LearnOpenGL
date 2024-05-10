#include "Buffers.h"

FrameBuffer::FrameBuffer(Texture)
{
	glCreateFramebuffers(1, &m_id);
	//glCheckFramebufferStatus
}