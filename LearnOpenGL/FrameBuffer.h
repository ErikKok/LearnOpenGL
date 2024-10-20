#pragma once

#include "Global.h"		// for UPtr
#include "Texture.h"	// for UPtr

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
	FrameBuffer(int x, int y);										// Constructor depthMap
	FrameBuffer(const FrameBuffer& other) = delete;					// Copy constructor
	FrameBuffer& operator=(const FrameBuffer& other) = delete;		// Copy assignment
	FrameBuffer(FrameBuffer&& other) noexcept = delete; 			// Move constructor	
	FrameBuffer& operator=(FrameBuffer&& other) noexcept = delete;	// Move assignment
	~FrameBuffer();													// Destructor
	
	unsigned int getId() const { return m_id; };
	const framebufferType& getType() const { return m_type; };

	Texture* getTexture() { return m_texture.get(); };
	const Texture* getTexture() const { return m_texture.get(); };
	// Or use Explicit object parameters (this is just here as an example) - https://devblogs.microsoft.com/cppblog/cpp23-deducing-this/
	//template <class Self>
	//auto&& getTexture(this Self&& self) {
	//	return std::forward<Self>(self).m_texture;
	//}

	void bind() const;
	void unbind() const;

private:
	GLuint m_id{};
	framebufferType m_type{ framebufferType::undefined };
	UPtr<Texture> m_texture{};
};