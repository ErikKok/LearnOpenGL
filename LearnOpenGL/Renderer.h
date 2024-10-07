#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glad/glad.h>

#include <memory> // for std::unique_ptr and std::make_unique

// TODO ECS - transparency opslaan in Material, of uit texture halen in constructor van een Material class?
struct Material {
	Shader* shader{ nullptr };
	int diffuse1{ 0 };					// sampler2D
	int specular1{ 0 };					// sampler2D
	int normal1{ 7 };					// sampler2D
	//int height1{ 0 };					// sampler2D
	float shininess{ 0.0f };
	int emissionTexture{ 0 };			// sampler2D
	float emissionStrength{ 0.0f };
	int lightEmissionMap{ 0 };			// sampler2D
	int lightEmissionTexture{ 0 };		// sampler2D
	bool enableGL_CULL_FACE{ true };	// set false for 2D objects
	// int cullFace{ GL_FRONT };			// false == GL_BACK
	// bool depthTested;
	// bool writeToDepthBuffer;
	// bool blendMode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RenderObject {
public:
	RenderObject(Mesh* mesh, Material* material, int instances = 1)
		: mesh{ mesh }
		, material{ material }
		, instances{ instances } // used to resize model array and BufferDataStore m_data in ShaderStorageBuffer
	{
		transform.resize(instances);
	}

	void addSSBO(int bindingPoint, GLsizeiptr elementSize, SSBO type = SSBO::undefined) {
		ssbo.emplace_back(std::make_unique<ShaderStorageBuffer>(bindingPoint, instances, elementSize, type));
	}

	Mesh* mesh{ nullptr }; // TODO ECS - should it own it's mesh? if you want a copy of a Mesh, the data members should be shared ptr's, not unique. // std::shared_ptr
	Material* material{ nullptr }; // TODO ECS - make unique_ptr van maken? // TODO should it own it's material?
	std::unique_ptr<Model> model{ nullptr };
	std::vector<glm::mat4> transform{};
	std::vector<std::unique_ptr<ShaderStorageBuffer>> ssbo; // Each RenderObject owns it's unique SSBOs (on the heap), this way you can upload them just once per renderpass (raw pointers (on the stack) are max 1% faster)
	int instances{ 1 };
	bool castsShadow{ true };
	bool drawOutline{ false };
	bool drawAsSingleColor{ false };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum renderPassType : int {
	undefined,
	normal,
	depthMap,
	depthMapDirLight,
	depthMapSpotLight0,
	depthMapSpotLight1,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Renderer {
public:
	const renderPassType& getRenderPassActive() const { return m_renderPassActive; };
	void setRenderPassActive(renderPassType type) { m_renderPassActive = type; };
	void createShaderSingleColor(std::string string) { m_shaderSingleColor = std::make_unique<Shader>(string); };
	void createShaderSkybox(std::string string) { m_shaderSkybox = std::make_unique<Shader>(string); };
	void createShaderFrustum(std::string string) { m_shaderFrustum = std::make_unique<Shader>(string); };
	void createShaderDebugQuad(std::string string) { m_shaderDebugQuad = std::make_unique<Shader>(string); };
	const Shader* const getShaderDebugQuad() const { return m_shaderDebugQuad.get(); };
	void createShaderDepthMap(std::string string) { m_shaderDepthMap = std::make_unique<Shader>(string); };

	//void isRendererComplete() const { assert(m_shaderDepthMapDirLight != nullptr || m_shaderDepthMapSpotLight1 != nullptr || m_shaderDepthMapSpotLight0 != nullptr || m_shaderSingleColor != nullptr || m_shaderSkybox != nullptr || m_shaderFrustum != nullptr || m_shaderDebugQuad != nullptr); };
	
	void clearColor() const { glClearColor(1.0f, 0.0f, 1.0f, 1.0f); }; // magenta
	void clearColorAndDepthBuffer() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
	void clearDepthBuffer() const { glClear(GL_DEPTH_BUFFER_BIT); }

	void draw(const RenderObject& RO) const;
	void drawSingleColor(const RenderObject& RO) const;
	void goRenderSkybox(const Mesh& mesh) const; // Assumes SkyBox Texture is already bound, and will never be changed
	void drawFrustum(const Mesh& mesh, const glm::mat4& viewProjectionMatrix) const;
	void drawDebugQuad(const Mesh& mesh, const Camera& useCamera) const; // Takes in a Camera, not an OrthograpicCamera!

	void clearStencilBuffer() const;
	void initStencilBuffer();
	void drawWithStencil(const RenderObject& RO);
	void goRender();
	void goRenderOutline();

	void setViewPort() { glViewport(0, 0, G::windowWidth, G::windowHeight); }
	void setViewPort(FrameBuffer* FBO) { glViewport(0, 0, FBO->getTexture()->getWidth(), FBO->getTexture()->getHeight()); }



	void submitRO(RenderObject RO) { m_renderVector.emplace_back(); }
	// TODO private
	std::vector<RenderObject*> m_renderVector{}; // TODO ECS - uPtr? or reference wrappers? of een shared ptr?
	std::vector<std::unique_ptr<FrameBuffer>> m_FBO{}; // TODO ECS - make setter

private:
	renderPassType m_renderPassActive{ renderPassType::undefined };
	std::unique_ptr<Shader> m_shaderSingleColor{ nullptr };
	std::unique_ptr<Shader> m_shaderSkybox{ nullptr };
	std::unique_ptr<Shader> m_shaderFrustum{ nullptr };
	std::unique_ptr<Shader> m_shaderDebugQuad{ nullptr };
	std::unique_ptr<Shader> m_shaderDepthMap{ nullptr };
};

//https://youtu.be/akxevYYWd9g?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
//renderCommand
//- clear
//- enzo
//
//beginScene(lights, camera);  environment stuff
// sene bevat alles voor de scene, dat veranderd niet zo vaak, pas als je verder beweegt bijv.
// scene kan voor nu fixed zijn
//
//submit(); Geometry, meshes, transforms
//
//- al deze submits komen in een queue
// als alles is gesubmit, dan:
//endScene();
//
//renderCommand
// nu alles voor de scene bekend is, gaan we alles orderen
//- order meshes, etc
//
//- draw everthing