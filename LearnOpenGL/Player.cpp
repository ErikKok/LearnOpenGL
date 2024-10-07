#pragma once

#include "Player.h"
//#include "GlobalEntities.h"

void Player::jump() {
	if (m_isJumping == false) { // + check if touching ground
		m_acceleration = 666.0f;
		m_isJumping = true;
	}
}

void Player::processInput(GLFWwindow* window)//, Camera camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //camera.processKeyboard(CameraMovement::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //GE::camera.processKeyboard(CameraMovement::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //GE::camera.processKeyboard(CameraMovement::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //GE::camera.processKeyboard(CameraMovement::RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        //GE::camera.processKeyboard(CameraMovement::UP);
        this->jump();
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        //GE::camera.processKeyboard(CameraMovement::DOWN);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        //GE::camera.setMovementSpeed(10.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        //GE::camera.setMovementSpeed(5.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}