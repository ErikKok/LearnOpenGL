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

FrameBuffer::FrameBuffer(Texture& texture)
	: m_type{ framebufferType::depthMap }
	, m_texture{ &texture }
{
	assert(texture.getType() == textureType::depthMap && "Texture has wrong Type for this constructor");
	
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

void FrameBuffer::startDepthMap(const Shader* shader) const
{
	assert(m_type == framebufferType::depthMap && "Wrong framebufferType");

	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	s_depthMapPassActive = true;
	glViewport(0, 0, m_texture->getWidth(), m_texture->getHeight());
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way // TODO to renderer
	shader->useShader();
	//shader.bindTexture(x);

	Global::glCheckError();
}

void FrameBuffer::stopDepthMap() const
{
	assert(m_type == framebufferType::depthMap && "Wrong framebufferType");
	
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Global::glCheckError();
	s_depthMapPassActive = false;
}