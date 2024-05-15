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

FrameBuffer::FrameBuffer(Texture& depthMap)
	: m_type{ framebufferType::depthMap }
	, m_depthMap{ &depthMap }
{
	assert(depthMap.getType() == textureType::depthMap && "Texture has wrong Type for this constructor");
	
	glCreateFramebuffers(1, &m_id);
	glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depthMap->getId(), 0);
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

void FrameBuffer::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	if (m_type == framebufferType::depthMap) {
		glViewport(0, 0, m_depthMap->getWidth(), m_depthMap->getHeight());
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	Global::glCheckError();
}

void FrameBuffer::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Global::glCheckError();
}