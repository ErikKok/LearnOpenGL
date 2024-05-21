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
	//const float getLeft() const { return m_left; };
	//const float getRight() const { return m_right; };
	//const float getBottom() const { return m_bottom; };
	//const float getTop() const { return m_top; };
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

	void bind();
	void unbind();

	void startDepthMap(Shader shader);
	void stopDepthMap();

	//std::array<float, 42> getArray()
	//{
	//	return {
	//		m_right, m_top,    m_farPlane,  // Back-top-right
	//		m_left,  m_top,    m_farPlane,  // Back-top-left
	//		m_right, m_bottom, m_farPlane,  // Back-bottom-right
	//		m_left,  m_bottom, m_farPlane,  // Back-bottom-left
	//		m_left,  m_bottom, m_nearPlane, // Front-bottom-left
	//		m_left,  m_top,    m_farPlane,  // Back-top-left
	//		m_left,  m_top,    m_nearPlane, // Front-top-left
	//		m_right, m_top,    m_farPlane,  // Back-top-right
	//		m_right, m_top,    m_nearPlane, // Front-top-right
	//		m_right, m_bottom, m_farPlane,  // Back-bottom-right
	//		m_right, m_bottom, m_nearPlane, // Front-bottom-right
	//		m_left,  m_bottom, m_nearPlane, // Front-bottom-left
	//		m_right, m_top,    m_nearPlane, // Front-top-right
	//		m_left,  m_top,    m_nearPlane, // Front-top-left
	//	
	//		//+0.5, +0.5, -0.5, // Back-top-right
	//		//-0.5, +0.5, -0.5, // Back-top-left
	//		//+0.5, -0.5, -0.5, // Back-bottom-right
	//		//-0.5, -0.5, -0.5, // Back-bottom-left
	//		//-0.5, -0.5, +0.5, // Front-bottom-left
	//		//-0.5, +0.5, -0.5, // Back-top-left
	//		//-0.5, +0.5, +0.5, // Front-top-left
	//		//+0.5, +0.5, -0.5, // Back-top-right
	//		//+0.5, +0.5, +0.5, // Front-top-right
	//		//+0.5, -0.5, -0.5, // Back-bottom-right
	//		//+0.5, -0.5, +0.5, // Front-bottom-right
	//		//-0.5, -0.5, +0.5, // Front-bottom-left
	//		//+0.5, +0.5, +0.5, // Front-top-right
	//		//-0.5, +0.5, +0.5, // Front-top-left
	//	};
	//};

private:
	GLuint m_id{};
	framebufferType m_type{ framebufferType::undefined };
	UPtr<Texture> m_texture{};
	float m_fov{ 90.0f };				// Used for Perspective ProjectionMatrix
	float m_left{ -20.0f };				// Used for Orthographic ProjectionMatrix
	float m_right{ 20.0f };				// Used for Orthographic ProjectionMatrix
	float m_bottom{ -20.0f };			// Used for Orthographic ProjectionMatrix
	float m_top{ 20.0f };				// Used for Orthographic ProjectionMatrix
	float m_nearPlane{ 0.1f };			// For Orthographic interpreted as distance along the view direction, for Perspective as distance front the camera
	float m_farPlane{ 100.0f };			// For Orthographic interpreted as distance along the view direction, for Perspective as distance front the camera
	glm::mat4 m_projectionMatrix{};
	glm::mat4 m_viewMatrix{};
	glm::mat4 m_viewProjectionMatrix{};
	bool m_orthographic{ false };		// false == perspective
};