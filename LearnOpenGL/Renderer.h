#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"

#include <memory> // for std::unique_ptr and std::make_unique

// TODO transparency opslaan in Material, of uit texture halen in constructor van een Material class?
struct Material {
	Shader& shader;
	int diffuse1{ 0 };						// sampler2D
	int specular1{ 0 };						// sampler2D
	//int normal1;							// sampler2D
	//int height1;							// sampler2D
	int emission{ 0 };						// sampler2D
	float emissionStrength{ 0.0f };
	float shininess{ 0.0f };
	int flashLightEmissionMap{ 0 };			// sampler2D
	int flashLightEmissionTexture{ 0 };		// sampler2D
};

enum class renderPassType {
	undefined,
	normal,
	depthMapDirLight,
	depthMapSpotLight,
	depthMapFlashLight,
};

class Renderer {
public:
	const renderPassType getRenderPassActive() const { return m_renderPassActive; };
	void setRenderPassActive(renderPassType type) { m_renderPassActive = type; };

	const Shader* getShaderDepthMapDirLight() const { return m_shaderDepthMapDirLight.get(); };
	void createShaderDepthMapDirLight(std::string string) { m_shaderDepthMapDirLight = std::make_unique<Shader>(string); };
	const Shader* getShaderDepthMapSpotLight() const { return m_shaderDepthMapSpotLight.get(); };
	void createShaderDepthMapSpotLight(std::string string) { m_shaderDepthMapSpotLight = std::make_unique<Shader>(string); };
	const Shader* getShaderDepthMapFlashLight() const { return m_shaderDepthMapFlashLight.get(); };
	void createShaderDepthMapFlashLight(std::string string) { m_shaderDepthMapFlashLight = std::make_unique<Shader>(string); };
	const Shader* getShaderSingleColor() const { return m_shaderSingleColor.get(); };
	void createShaderSingleColor(std::string string) { m_shaderSingleColor = std::make_unique<Shader>(string); };
	void createShaderSkybox(std::string string) { m_shaderSkybox = std::make_unique<Shader>(string); };
	void createShaderFrustum(std::string string) { m_shaderFrustum = std::make_unique<Shader>(string); };
	void createShaderDebugQuad(std::string string) { m_shaderDebugQuad = std::make_unique<Shader>(string); };

	void clear() const;

	// TODO store the vao/ebo's/meshes etc in a list/batch/whatever, order them, then batch render them
	// void draw(RederBatch);
	void draw(const Mesh& mesh, const Material& material, GLsizei instances = 1) const;												
	void drawSingleColor(const Mesh& mesh, const glm::vec4 color, GLsizei instances = 1) const;

	// Assumes SkyBox Texture is already bound, and will never be changed
	void drawSkybox(const Mesh& mesh) const;
	void drawFrustum(const Mesh& mesh, const glm::mat4& viewProjectionMatrix) const;
	// Takes in a Camera, not an OrthograpicCamera!
	void drawDebugQuad(const Mesh& mesh, const Camera& useCamera) const;
	void drawModel(const Mesh& mesh, const Material& material) const;

	// non-DSA
	//void draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances = 1) const;		    
	//void drawXYZ(ShaderStorageBuffer& ssbo) const;

private:
	renderPassType m_renderPassActive{ renderPassType::undefined };

	std::unique_ptr<Shader> m_shaderDepthMapDirLight{ nullptr }; //before we use either of these operators, we should check whether the std::unique_ptr actually has a resource
	std::unique_ptr<Shader> m_shaderDepthMapSpotLight{ nullptr };
	std::unique_ptr<Shader> m_shaderDepthMapFlashLight{ nullptr };
	std::unique_ptr<Shader> m_shaderSingleColor{ nullptr };
	std::unique_ptr<Shader> m_shaderSkybox{ nullptr };
	std::unique_ptr<Shader> m_shaderFrustum{ nullptr };
	std::unique_ptr<Shader> m_shaderDebugQuad{ nullptr };

	//bool glStencilFuncActive{};
	//bool glDepthTestActive{}; // ?

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
};