#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glad/glad.h>

#include <memory> // for std::unique_ptr and std::make_unique

// TODO transparency opslaan in Material, of uit texture halen in constructor van een Material class?
struct Material {
	Shader& shader;
	int diffuse1{ 0 };					// sampler2D
	int specular1{ 0 };					// sampler2D
	int normal1{ 7 };					// sampler2D
	//int height1{ 0 };					// sampler2D
	float shininess{ 0.0f };
	int emissionTexture{ 0 };			// sampler2D
	float emissionStrength{ 0.0f };
	int lightEmissionMap{ 0 };			// sampler2D
	int lightEmissionTexture{ 0 };		// sampler2D
	bool enableGL_CULL_FACE{ true };
	//int cullFace{ GL_FRONT };			// false == GL_BACK
	// bool depthTested;
	// bool writeToDepthBuffer;
	// bool blendMode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RenderObject {
public:
	RenderObject(Mesh* mesh = nullptr, Material* material = nullptr, int instances = 1)
		: mesh{ mesh }
		, material{ material }
		, instances{ instances } // used to resize model array and BufferDataStore m_data in ShaderStorageBuffer
	{
		model.resize(instances);
	}

	void addSSBO(int bindingPoint, GLsizeiptr elementSize) {
		ssbo.emplace_back(std::make_unique<ShaderStorageBuffer>(bindingPoint, instances, elementSize));
	}

	Mesh* mesh{ nullptr }; // TODO should it own it's mesh?
	Material* material{ nullptr }; // TODO make unique_ptr van maken? // TODO should it own it's material?
	std::vector<glm::mat4> model{}; // transforms
	std::vector<std::unique_ptr<ShaderStorageBuffer>> ssbo; // Each RenderObject owns it's unique SSBOs (on the heap), this way you can upload them just once per renderpass (raw pointers (on the stack) are max 1% faster)
	int instances{ 1 };
	// renderType type; (transparant, singleColor, isModel, etc.;
	// bool isSelected; true = de outline renderen
	// bool castShadow;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class renderPassType {
	undefined,
	normal,
	depthMapDirLight,
	depthMapSpotLight,
	depthMapFlashLight,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	void isRendererComplete() { assert(m_shaderDepthMapDirLight != nullptr || m_shaderDepthMapSpotLight != nullptr || m_shaderDepthMapFlashLight != nullptr || m_shaderSingleColor != nullptr || m_shaderSkybox != nullptr || m_shaderFrustum != nullptr || m_shaderDebugQuad != nullptr); };
	void clear() const;

	// TODO store the vao/ebo's/meshes/RO's etc in a list/batch/whatever, order them, then batch render them
	// void draw(RenderBatch);
	void draw(const RenderObject& RO) const;
	void drawModel(const RenderObject& RO, Model& model);
	void drawSingleColor(const RenderObject& RO) const;
	void drawSkybox(const Mesh& mesh) const; // Assumes SkyBox Texture is already bound, and will never be changed
	void drawFrustum(const Mesh& mesh, const glm::mat4& viewProjectionMatrix) const;
	void drawDebugQuad(const Mesh& mesh, const Camera& useCamera) const; // Takes in a Camera, not an OrthograpicCamera!

	// OLD
	//void draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances = 1) const;		    
	//void drawXYZ(ShaderStorageBuffer& ssbo) const;
	//void draw(const Mesh& mesh, const Material& material, GLsizei instances = 1) const;												
	//void drawSingleColor(const Mesh& mesh, const glm::vec4 color, GLsizei instances = 1) const;

private:
	renderPassType m_renderPassActive{ renderPassType::undefined };

	std::unique_ptr<Shader> m_shaderDepthMapDirLight{ nullptr }; 
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

//renderCommand
//- clear
//- enzo
//
//beginScene(lights, camera);  environment stuff
//
//submit(); Geometry, meshes, transforms
//
//- queue in RenderCommand
//
//endScene();
//
//renderCommand
//- order meshes
//
//- draw everthing