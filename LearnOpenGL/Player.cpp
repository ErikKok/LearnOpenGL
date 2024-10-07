#pragma once

#include "Player.h"
//#include "GlobalEntities.h"

void Player::jump() {
	if (m_isJumping == false) { // + check if touching ground
		m_acceleration = 666.0f;
		m_isJumping = true;
	}
}