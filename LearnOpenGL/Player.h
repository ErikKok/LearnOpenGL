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
	const glm::vec3 getSpeed() const { return m_speed; };
	void setSpeed(glm::vec3 x) { m_speed = x; };
	const glm::vec3 getSpeedLastFrame() const { return m_speedLastFrame; };

	//const float getMaxCurrentSpeed() const { return m_maxCurrentSpeed; };
	void setMaxCurrentSpeed(float x) { m_maxCurrentSpeed = x; };
	const float getWalkSpeed() const { return m_walkSpeed; };
	const float getRunSpeed() const { return m_runSpeed; };

	void setMaxStrafeCurrentSpeed(float x) { m_maxStrafeCurrentSpeed = x; };
	const float getStrafeWalkSpeed() const { return m_StrafeWalkSpeed; };
	const float getStrafeRunSpeed() const { return m_StrafeRunSpeed; };

	const glm::vec3 getAcceleration() const { return m_acceleration; };
	void setAcceleration(glm::vec3 x) { m_acceleration = x; };

	// Gravity + jumping documentation
	// https://gafferongames.com/post/integration_basics/
	// https://gamedev.stackexchange.com/questions/94000/how-to-implement-accurate-frame-rate-independent-physics
	// https://gamedev.stackexchange.com/questions/38453/how-do-i-implement-deceleration-for-the-player-character
	
	void calculateValues();						// recalculates all/most values used with physicsFrameTime, use after changing physicsFrameTime
	void calculateForwardSpeed();
	void calculateRightSpeed();
	void initMovement(PlayerMovement direction);
	// doPhysics start
	//void limitAcceleration();
	void calculateSpeed();
	void limitSpeed();
	void handleJump();
	void updatePosition();
	void resetAcceleration();
	// doPhysics end
	AABB getTAABB();
	AABB getTAABB(glm::vec3& proposedPosition);

	void calculateDirection();					// only used for ImGui & limitSpeed as test

private:
	glm::vec3 m_speed{ 0.0f };					// meter/second TODO
	glm::vec3 m_speedLastFrame{ 0.0f };
	float m_forwardSpeed{ 0.0f };				// https://forums.unrealengine.com/t/character-movement-direction/30708/4
	float m_rightSpeed{ 0.0f };

	float m_maxCurrentSpeed{ 22.0f };			// limits speed in 1 axis, total speed could get higher
	float m_walkSpeed{ 5.0f };
	float m_runSpeed{ 22.0f };
	float m_maxStrafeCurrentSpeed{ 15.55f };	// Given a=15.55 and b=15.55, c = squared 483.605 = 21.996
	float m_StrafeWalkSpeed{ 3.5f };
	float m_StrafeRunSpeed{ 16.0f };
	float m_maxJumpSpeed{ 200.0f };

	glm::vec3 m_acceleration{ glm::vec3(0.0f, -G::gravity, 0.0f) };
	float m_WalkAcceleration{ 2.500f };			// XZ
	float m_StrafeAcceleration{ 2.000f };
	float m_AirborneAcceleration{ 1.500f };		// XZ
	float m_jumpAcceleration{ 3.000f };			// Y

	//float m_maxAcceleration{ 800.0f };
	//float m_maxJumpAcceleration{ 800.0f };
	//float m_maxJumpStrafeAcceleration{ 2.8f };

	float m_dryFriction{ 0.92f };
	float m_aeroDrag{ 0.9980f };
	float m_gravityBoost{ 3.5f };				// "speeds up" gravity while jumping, makes it less floaty

	//bool m_isRunning{ false };
	bool m_isAirborne{ false };

	glm::vec3 m_direction{ 0.0f };
	glm::vec3 m_positionLastFrame{ G::cameraInitialPosition };
};