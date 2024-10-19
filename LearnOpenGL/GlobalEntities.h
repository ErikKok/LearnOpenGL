#pragma once

#include "Global.h"
#include "Camera.h"
#include "Player.h"

//class HesGui;
// Global Entities
namespace GE {
	inline Player player;
	inline Camera camera((static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)), G::cameraInitialPosition);
	//inline HesGui hesgui;
}

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_glfw.h"
#include "imgui-docking/imgui_impl_opengl3.h"

class HesGui
{
public:
	void bla() {
		ImGui::SliderFloat("m_maxCurrentSpeed", &GE::player.m_maxCurrentSpeed, 0.0f, 200.0f, "%.1f");
		ImGui::SliderFloat("m_maxJumpSpeed", &GE::player.m_maxJumpSpeed, 0.0f, 2000.0f, "%.1f");
		ImGui::SliderFloat("m_jumpAcceleration", &GE::player.m_jumpAcceleration, 0.0f, 2000.0f, "%.1f");
		ImGui::SliderFloat("m_maxJumpAcceleration", &GE::player.m_maxJumpAcceleration, 0.0f, 2000.0f, "%.1f");
	};
	void IsWindowFocused() {
		ImGui::IsWindowFocused();

	};

};