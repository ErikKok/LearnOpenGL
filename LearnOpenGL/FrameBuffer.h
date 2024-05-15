#pragma once

#include <array>

#include <glad/glad.h>

class Texture;

enum class framebufferType {
	undefined,
	depthMap,
	max_framebufferTypes,
};

constexpr std::array framebufferTypeName{ "undefined", "depthMap" };
static_assert(std::size(framebufferTypeName) == static_cast<int>(framebufferType::max_framebufferTypes));

class FrameBuffer {
public:
	FrameBuffer();													// Constructor
	FrameBuffer(Texture& depthMap);									// Constructor depthMap // TODO make const
	FrameBuffer(const FrameBuffer& other) = delete;					// Copy constructor
	FrameBuffer& operator=(const FrameBuffer& other) = delete;		// Copy assignment
	FrameBuffer(FrameBuffer&& other) noexcept = delete; 			// Move constructor	
	FrameBuffer& operator=(FrameBuffer&& other) noexcept = delete;	// Move assignment
	~FrameBuffer();													// Destructor

	void bindFrameBuffer();
	void unbindFrameBuffer();

	const unsigned int getId() const { return m_id; };

private:
	GLuint m_id{};
	framebufferType m_type{ framebufferType::undefined };
	SPtr<Texture> m_depthMap{};
	//glm::mat4 view{};?
	//glm::mat4 projection{};?
};


// class GL_SHADER_STORAGE_BUFFER