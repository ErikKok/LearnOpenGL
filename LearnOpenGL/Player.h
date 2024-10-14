#pragma once

#include "Engine.h"
#include "Global.h"

class Player
{
public:
	//const float getYVelocity() const { return m_yVelocity; };
	//void addYVelocity(float x) { m_yVelocity += x; };
	//void setYVelocity(float x) { m_yVelocity = x; };
	//const float getYVelocityLastFrame() const { return m_yVelocityLastFrame; };
	//void setYVelocityLastFrame(float x) { m_yVelocityLastFrame = x; };
	//const float getAcceleration() const { return m_acceleration; };
	//void multiplyAcceleration(float x) { m_acceleration *= x; };
	//void setAcceleration(float x) { m_acceleration = x; };
	//const float getDrag() const { return m_drag; };
	//const bool getIsJumping() const { return m_isJumping; };
	//void setIsJumping(bool x) { m_isJumping = x; };

	void setMaxCurrentSpeed(float x) { maxCurrentSpeed = x; };

	// Gravity + jumping documentation
	// https://gafferongames.com/post/integration_basics/
	// https://gamedev.stackexchange.com/questions/94000/how-to-implement-accurate-frame-rate-independent-physics
	// https://gamedev.stackexchange.com/questions/38453/how-do-i-implement-deceleration-for-the-player-character
	
	//void calculateDirection();
	void initMovement(CameraMovement direction);
	void initJump();
	void limitAcceleration();
	void limitSpeed();
	void handleMovement();
	void handleJump();
	void handleJumpextrapolation();
	AABB getTAABB();


private:
	glm::vec3 m_Speed{ 0.0f }; // Vertical speed in meter/second
	glm::vec3 m_SpeedLastFrame{ 0.0f }; //  meter/second
	//glm::vec3 m_positionLastFrame{ 0.0f };
	//glm::vec3 m_direction{ 0.0f };
	glm::vec3 m_acceleration{ glm::vec3(0.0f, -G::gravity, 0.0f) };
	float maxAcceleration{ 200.0f };
	float maxJumpAcceleration{ 120.0f };
	//float floatAcceleration{ 120.0f };
	float maxCurrentSpeed{ 18.0f };
	float dryFriction{ 85.0f };
	float m_aeroDrag{ 65.0f };
	float correction{};

	bool m_isRunning{ false };
	bool m_isJumping{ false };
	//bool m_isCrouching{ false };
};