#pragma once

class Player {
public:
	const float getYVelocity() const { return m_yVelocity; };
	void addYVelocity(float x) { m_yVelocity += x; };
	void setYVelocity(float x) { m_yVelocity = x; };
	const float getYVelocityLastFrame() const { return m_yVelocityLastFrame; };
	void setYVelocityLastFrame(float x) { m_yVelocityLastFrame = x; };
	const float getAcceleration() const { return m_acceleration; };
	void multiplyAcceleration(float x) { m_acceleration *= x; };
	void setAcceleration(float x) { m_acceleration = x; };
	const float getDrag() const { return m_drag; };
	const bool getIsJumping() const { return m_isJumping; };
	void setIsJumping(bool x) { m_isJumping = x; };

	void jump();

private:
	float m_yVelocity{ 0.0f }; // Vertical speed in meter/second
	float m_yVelocityLastFrame{ 0.0f };
	float m_acceleration{ 9.81f }; // TODO use -G::gravity value (but generates a header recursion)
	float m_drag{ 0.5f };
	bool m_isJumping{ false };

	//float m_yPositionLastFrame{ 0.0f };
	//bool m_isCrouching{ false };
};