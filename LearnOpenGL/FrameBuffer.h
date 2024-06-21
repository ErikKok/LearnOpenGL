#pragma once

#include "Global.h"
#include "Texture.h" // for UPtr

#include <array>

#include <glad/glad.h>

// aka depthMap class

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
	FrameBuffer(Texture& texture);									// Constructor depthMap // TODO make const
	FrameBuffer(const FrameBuffer& other) = delete;					// Copy constructor
	FrameBuffer& operator=(const FrameBuffer& other) = delete;		// Copy assignment
	FrameBuffer(FrameBuffer&& other) noexcept = delete; 			// Move constructor	
	FrameBuffer& operator=(FrameBuffer&& other) noexcept = delete;	// Move assignment
	~FrameBuffer();													// Destructor
	
	const unsigned int getId() const { return m_id; };
	const bool getOrthographic() const { return m_orthographic; };
	void setOrthographic(bool x) { m_orthographic = x; };

	void bind() const;
	void unbind() const;

	void startDepthMap(const Shader* shader) const;
	void stopDepthMap() const;

private:
	GLuint m_id{};
	framebufferType m_type{ framebufferType::undefined };
	UPtr<Texture> m_texture{};
	bool m_orthographic{ false }; // false == perspective  // TODO could be synced with camera type
};