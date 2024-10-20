//#pragma once
//
////#include "Camera.h"
//#include "Engine.h"
////#include "Global.h"
////#include "GlobalEntities.h"
////#include "Player.h"
//
//#include "imgui-docking/imgui.h"
//#include "imgui-docking/imgui_impl_glfw.h"
//#include "imgui-docking/imgui_impl_opengl3.h"
//
//namespace Im {
//
//    void Gui() {
//        ImGui::Begin("ImGui Window");
//
//        ImGuiIO& io = ImGui::GetIO();
//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//
//        ImGui::SliderFloat("m_maxJumpSpeed", &GE::player.m_maxJumpSpeed, 0.0f, 2000.0f, "%.1f");
//        ImGui::SliderFloat("m_jumpAcceleration", &GE::player.m_jumpAcceleration, 0.0f, 2000.0f, "%.1f");
//        ImGui::SliderFloat("m_maxJumpAcceleration", &GE::player.m_maxJumpAcceleration, 0.0f, 2000.0f, "%.1f");
//        ImGui::SliderFloat("m_dryFriction", &GE::player.m_dryFriction, 0.0f, 100.0f, "%.1f");
//
//        ImGui::Text("m_position = %f", GE::camera.m_position);
//        ImGui::Text("m_front = %f", GE::camera.m_front);
//
//        // call during physicsloop / split in 2 
//
//        ImGui::Text("ticksLoop = %d", Engine::ticksLoop);
//        ImGui::Text("ticksPhysics = %d", Engine::ticksPhysics);
//
//        ImGui::Text("m_speed = %.2f, %.2f, %.2f", GE::player.m_speed.x, GE::player.m_speed.y, GE::player.m_speed.z);
//        ImGui::Text("m_maxCurrentSpeed = %.2f", GE::player.m_maxCurrentSpeed);
//        ImGui::Text("m_forwardSpeed = %.2f", GE::player.m_forwardSpeed);
//        ImGui::Text("m_acceleration = 000%.2f, 000%.2f, 000%.2f", GE::player.m_acceleration.x, GE::player.m_acceleration.y, GE::player.m_acceleration.z);
//
//        // %d int
//        // %g insignificant zeroes to the right of the decimal point are not included
//
//        ImGui::End();
//    };
//}