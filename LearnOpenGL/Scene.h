//double totalTimePassed = 0.0f;
//double physicsTime = 0.01f; // 100fps
//
//double currentFrameTime = hires_time_in_seconds();
//double frameTimeRemaining = 0.0f;
//
//while (true)
//{
//    double newTime = time();
//    double frameTime = newTime - currentFrameTime;
//    if (frameTime > 0.25f)
//        frameTime = 0.25f;
//    currentFrameTime = newTime;
//
//    frameTimeRemaining += frameTime;
//
//    while (frameTimeRemaining >= physicsTime)
//    {
//        previousState = currentState;
//        //integrate(currentState, totalTimePassed, physicsTime); // do physics
//        doPhysics() // using the fixed physicsTime
//        totalTimePassed += physicsTime;
//        frameTimeRemaining -= physicsTime;
//    }
//
//    const double interpolationFactor = frameTimeRemaining / physicsTime;
//
//    float/double interpolationResult = currentState * interpolationFactor + previousState * (1.0 - interpolationFactor);
//    //           10 * 0.5 + 8 * (0.5) = 5 + 4 = 9
//    
//    render(interpolationResult);
//}


//#pragma once
//
//#include "Global.h"
//
//#include <entt/entt.hpp>
////#include <entt/entity/registry.hpp>
//
//struct TransformComponent {
//	glm::mat4 transform{};
//};
//
//
//class Scene {
//public:
//	Scene() {
//	
//		entt::entity entity = m_registry.create();
//	
//		TransformComponent& transform = m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
//	};
//
//
//private:
//	entt::registry m_registry;
//};