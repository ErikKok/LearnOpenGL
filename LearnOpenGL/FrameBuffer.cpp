#pragma once

#include "FrameBuffer.h"
#include "Global.h"
#include "Texture.h"

#include <print>

FrameBuffer::FrameBuffer()
{
	glCreateFramebuffers(1, &m_id);

	assert((glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	Global::glCheckError();

	std::println("CREATE FrameBuffer id: {}", m_id);
}

FrameBuffer::FrameBuffer(int x, int y)
	: m_type{ framebufferType::depthMap }
	, m_texture{ std::make_unique<Texture>(textureType::depthMap, x, y) }
{
	glCreateFramebuffers(1, &m_id);
	glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_texture->getId(), 0);
	glNamedFramebufferDrawBuffer(m_id, GL_NONE);
	glNamedFramebufferReadBuffer(m_id, GL_NONE);

	assert((glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	Global::glCheckError();

	std::println("CREATE DepthMapFrameBuffer id: {}", m_id);
}

FrameBuffer::~FrameBuffer()
{
	std::println("DELETE framebuffer id: {}", m_id);
	glDeleteFramebuffers(1, &m_id);
	Global::glCheckError();
}

void FrameBuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	Global::glCheckError();
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Global::glCheckError();
}