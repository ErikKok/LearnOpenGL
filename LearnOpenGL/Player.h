#pragma once

#include "Global.h"
#include "Engine.h" // for struct AABB

enum class PlayerMovement {
	forward,
	backward,
	left,
	right,
	up,
	down,
	forwardbackward,    // FORWARD && BACKWARD pressed together
	leftright,          // LEFT && RIGHT pressed together
	updown,             // UP && DOWN pressed together
	jump,
};

class Player
{
	friend void G::ImGui();

public:
	const float getMaxCurrentSpeed() const { return m_maxCurrentSpeed; };
	void setMaxCurrentSpeed(float x) { m_maxCurrentSpeed = x; };
	const glm::vec3 getSpeed() const { return m_speed; };
	void setSpeed(glm::vec3 x) { m_speed = x; };
	const glm::vec3 getSpeedLastFrame() const { return m_speedLastFrame; };
	const float getWalkSpeed() const { return m_walkSpeed; };
	const float getRunSpeed() const { return m_runSpeed; };

	// Gravity + jumping documentation
	// https://gafferongames.com/post/integration_basics/
	// https://gamedev.stackexchange.com/questions/94000/how-to-implement-accurate-frame-rate-independent-physics
	// https://gamedev.stackexchange.com/questions/38453/how-do-i-implement-deceleration-for-the-player-character
	
	void calculateForwardSpeed();
	//void calculateRightSpeed();
	void initMovement(PlayerMovement direction);
	// doPhysics start
	void limitAcceleration();
	void calculateSpeed();
	void limitSpeed();
	void handleJump();
	void resetAcceleration();

	AABB getTAABB();
	AABB getTAABB(glm::vec3& proposedPosition);
	//void initDirection();
	//void calculateDirection();
	glm::vec3 m_speed{ 0.0f };				// meter/second TODO
private:

	glm::vec3 m_speedLastFrame{ 0.0f };
	float m_forwardSpeed{ 0.0f };			// only updated when needed // https://forums.unrealengine.com/t/character-movement-direction/30708/4
	//float m_rightSpeed{ 0.0f };
	float m_maxCurrentSpeed{ 22.0f };		// limits speed in 1 axis, total speed could get higher
	float m_walkSpeed{ 5.0f };
	float m_runSpeed{ 22.0f };
	float m_maxJumpSpeed{ 250.0f };
	//float m_maxStrafeRunSpeed{ 18.0f }; // TODO? + m_maxStrafeWalkSpeed

	glm::vec3 m_acceleration{ glm::vec3(0.0f, -G::gravity, 0.0f) };
	float m_WalkAcceleration{ 350.0f };
	float m_StrafeAcceleration{ 250.0f };
	float m_jumpAcceleration{ 300.0f };
	float m_maxAcceleration{ 800.0f };
	float m_maxJumpAcceleration{ 800.0f };
	float m_maxJumpSidewaysAcceleration{ 2.8f };

	float m_airborneDecelerationFactor{ 75.00f };
	float m_dryFriction{ 85.0f };
	float m_aeroDrag{ 99.80f };
	float m_gravityBoost{ 3.5f }; // "speeds up" gravity while jumping

	bool m_isRunning{ false };
	bool m_isAirborne{ false };

	//glm::vec3 m_positionLastFrame{ 0.0f };
	//glm::vec3 m_direction{ 0.0f };
};