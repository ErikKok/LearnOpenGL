#pragma once

#include "Global.h"
#include "Camera.h"
#include "Player.h"

// Global Entities
namespace GE {
	inline Player player;
	inline Camera camera((static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)), G::cameraInitialPosition);
}