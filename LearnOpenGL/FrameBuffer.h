#pragma once

#include "Global.h"
#include "Texture.h" // for UPtr

#include <array>

#include <glad/glad.h>

enum class framebufferType {
	undefined,
	depthMap,
	max_framebufferTypes,
};

constexpr std::array framebufferTypeName{ "undefined", "depthMap" };
static_assert(std::size(framebufferTypeName) == static_cast<int>(framebufferType::max_framebufferTypes));

class FrameBuffer {
public:
	static inline bool s_depthMapPassActive{ false };

	FrameBuffer();													// Constructor
	FrameBuffer(Texture& texture);									// Constructor depthMap // TODO make const
	FrameBuffer(const FrameBuffer& other) = delete;					// Copy constructor
	FrameBuffer& operator=(const FrameBuffer& other) = delete;		// Copy assignment
	FrameBuffer(FrameBuffer&& other) noexcept = delete; 			// Move constructor	
	FrameBuffer& operator=(FrameBuffer&& other) noexcept = delete;	// Move assignment
	~FrameBuffer();													// Destructor
	
	const unsigned int getId() const { return m_id; };
	const float getFov() const { return m_fov; };
	void setFov(float x) { m_fov = x; };
	const float getNearPlane() const { return m_nearPlane; };
	void setNearPlane(float x) { m_nearPlane = x; };
	const float getFarPlane() const { return m_farPlane; };
	void setFarPlane(float x) { m_farPlane = x; };
	const bool getOrthographic() const { return m_orthographic; };
	void setOrthographic(bool x) { m_orthographic = x; };

	// Compute the ModelView matrix from the light's point of view
	// projection matrix determines the dimensions of the view cuboid
	const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; };
	//void setProjectionMatrix(glm::mat4 mat4) { m_projectionMatrix = mat4; };
	void calculateProjectionMatrixOrthographic() { m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, -m_nearPlane, m_farPlane); };
	void calculateProjectionMatrixPerspective(Texture& texture) { m_projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(texture.getWidth() / texture.getHeight()), m_nearPlane, m_farPlane); };
	// view matrix determines it's position and orientation in the world
	// the cuboid needs to line up with the lights direction       
	const glm::mat4 getViewMatrix() const { return m_viewMatrix; };
	void setViewMatrix(glm::mat4 mat4) { m_viewMatrix = mat4; };
	const glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; };
	void calculateViewProjectionMatrix() { m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix; };

	void bindFrameBuffer();
	void unbindFrameBuffer();

	void initDepthMapFrameBuffer(Shader shader);
	void deinitDepthMapFrameBuffer();

private:
	GLuint m_id{};
	framebufferType m_type{ framebufferType::undefined };
	UPtr<Texture> m_texture{};
	float m_fov{ 90.0f };
	float m_left{ -20.0f };
	float m_right{ 20.0f };
	float m_bottom{ -20.0f };
	float m_top{ 20.0f };
	float m_nearPlane{ 0.1f };
	float m_farPlane{ 100.0f };
	glm::mat4 m_projectionMatrix{};
	glm::mat4 m_viewMatrix{};
	glm::mat4 m_viewProjectionMatrix{};
	bool m_orthographic{ false }; // true == perspective
};
