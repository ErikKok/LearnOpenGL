#pragma once

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

	// Gravity + jumping documentation
	// https://gafferongames.com/post/integration_basics/
	// https://gamedev.stackexchange.com/questions/94000/how-to-implement-accurate-frame-rate-independent-physics
	// https://gamedev.stackexchange.com/questions/38453/how-do-i-implement-deceleration-for-the-player-character
	void initJump();
	void handleJump();
	void handleJumpInterpolation();

private:
	float m_ySpeed{ 0.0f }; // Vertical speed in meter/second
	float m_ySpeedLastFrame{ 0.0f }; //  meter/second

	float m_acceleration{ -G::gravity };
	float m_drag{ 0.5f };
	bool m_isJumping{ false };

	//float m_yPositionLastFrame{ 0.0f };
	//bool m_isCrouching{ false };
};