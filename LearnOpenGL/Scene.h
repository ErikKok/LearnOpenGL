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

//struct RenderObject {
//	mesh;
//	material;
//	vector<model / transforms>;
//	mat4 dirLightMVPMatrixSSBO;
//	vector<spotLightMVPMatrixSSBO>;
//	vector<flashLightMVPMatrixSSBO>;
//	renderType type; (transparant, singleColor, etc.;
//	bool isSelected; true = de outline renderen
//};