#pragma once

#include "Engine.h"
#include "Global.h"

class Player
{
public:
	const float getMaxCurrentSpeed() const { return m_maxCurrentSpeed; };
	void setMaxCurrentSpeed(float x) { m_maxCurrentSpeed = x; };
	const glm::vec3 getSpeed() const { return m_speed; };

	// Gravity + jumping documentation
	// https://gafferongames.com/post/integration_basics/
	// https://gamedev.stackexchange.com/questions/94000/how-to-implement-accurate-frame-rate-independent-physics
	// https://gamedev.stackexchange.com/questions/38453/how-do-i-implement-deceleration-for-the-player-character
	
	void initMovement(CameraMovement direction);
	void limitAcceleration();
	void limitSpeed();
	void handleMovement();
	void resetAcceleration();
	AABB getTAABB();
	//void calculateDirection();

private:
	glm::vec3 m_speed{ 0.0f };										// meter/second
	glm::vec3 m_speedLastFrame{ 0.0f };								// meter/second
	float m_maxCurrentSpeed{ 22.0f };
	float m_maxJumpSpeed{ 100.0f };
	//float m_maxStrafeSpeed{ 18.0f };								// TODO
	
	//glm::vec3 m_positionLastFrame{ 0.0f };
	//glm::vec3 m_direction{ 0.0f };

	glm::vec3 m_acceleration{ glm::vec3(0.0f, -G::gravity, 0.0f) };
	float m_maxAcceleration{ 800.0f };
	float m_maxJumpAcceleration{ 500.0f };

	float m_airborneDecelerationFactor{ 0.75f };
	float m_dryFriction{ 85.0f };
	float m_aeroDrag{ 99.75f };
	float m_correction{ 0.0f };

	bool m_isRunning{ false };
	bool m_isAirborne{ false };
};