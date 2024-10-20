#pragma once

#include "Camera.h"
#include "Player.h"
#include "Global.h"

namespace GE {
	inline Player player;
	inline Camera camera((static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)), G::cameraInitialPosition);
}