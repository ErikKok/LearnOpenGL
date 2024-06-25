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
		modelTransform.resize(instances);
	}

	void addSSBO(int bindingPoint, GLsizeiptr elementSize) {
		ssbo.emplace_back(std::make_unique<ShaderStorageBuffer>(bindingPoint, instances, elementSize));
	}

	Mesh* mesh{ nullptr }; // TODO should it own it's mesh?
	Material* material{ nullptr }; // TODO make unique_ptr van maken? // TODO should it own it's material?
	Model* model{ nullptr };
	std::vector<glm::mat4> modelTransform{}; // transforms
	std::vector<std::unique_ptr<ShaderStorageBuffer>> ssbo; // Each RenderObject owns it's unique SSBOs (on the heap), this way you can upload them just once per renderpass (raw pointers (on the stack) are max 1% faster)
	int instances{ 1 };
	bool drawShadow{ true };
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
	void clear() const; //  color and depth

	// TODO store the vao/ebo's/meshes/RO's etc in a list/batch/whatever, order them, then batch render them
	// void draw(RenderBatch);
	void draw(const RenderObject& RO) const;
	void drawModel(const RenderObject& RO, Model& model);
	void drawSingleColor(const RenderObject& RO) const;
	void drawSkybox(const Mesh& mesh) const; // Assumes SkyBox Texture is already bound, and will never be changed
	void drawFrustum(const Mesh& mesh, const glm::mat4& viewProjectionMatrix) const;
	void drawDebugQuad(const Mesh& mesh, const Camera& useCamera) const; // Takes in a Camera, not an OrthograpicCamera!

	void initStencilBuffer();
	void clearStencilBuffer();
	void clearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT);	}
	void drawWithStencil(const RenderObject& RO);
	void drawOutline(RenderObject& RO);

	void setViewPort(FrameBuffer* FBO) {
		glViewport(0, 0, FBO->getTexture()->getWidth(), FBO->getTexture()->getHeight());
	}

	std::vector<RenderObject*> m_renderVector{}; // TODO uPtr?

	void goRender() {
		clearStencilBuffer();

		glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way // TODO to renderer

		m_renderPassActive = renderPassType::depthMapDirLight;
		assert(m_FBODirLight && "FBO is nullptr");
		assert(m_FBODirLight->getType() == framebufferType::depthMap && "Wrong framebufferType");

		m_FBODirLight->bind();
		setViewPort(m_FBODirLight.get());
		clearDepthBuffer();
		m_shaderDepthMapDirLight->useShader();

		for (const auto& RO : m_renderVector) {
			//if (RO.drawShadow && RO.model)
			//	drawModel(RO, &RO.model);
			if (RO->drawShadow)
				draw(*RO);
		}

		m_FBODirLight->unbind();

		m_renderPassActive = renderPassType::depthMapFlashLight;
		assert(m_FBOFlashLight && "FBO is nullptr");
		assert(m_FBOFlashLight->getType() == framebufferType::depthMap && "Wrong framebufferType");

		if (SpotLight::spotLights[0].getOn()) {
			m_FBOFlashLight->bind();
			setViewPort(m_FBOFlashLight.get());
			clearDepthBuffer();
			m_shaderDepthMapFlashLight->useShader();

			for (const auto& RO : m_renderVector) {
				if (RO->drawShadow)
					draw(*RO);
			}

			m_FBOFlashLight->unbind();
		}

		m_renderPassActive = renderPassType::depthMapSpotLight;
		assert(m_FBOSpotLight && "FBO is nullptr");
		assert(m_FBOSpotLight->getType() == framebufferType::depthMap && "Wrong framebufferType");


		
		m_FBOSpotLight->bind();
		setViewPort(m_FBOSpotLight.get());
		clearDepthBuffer();
		m_shaderDepthMapSpotLight->useShader();

		for (const auto& RO : m_renderVector) {
			if (RO->drawShadow)
				draw(*RO);
		}


		m_FBOSpotLight->unbind();


		glCullFace(GL_BACK);

		m_renderPassActive = renderPassType::normal;

		glViewport(0, 0, Global::windowWidth, Global::windowHeight);
		clear();

		

		for (const auto& RO : m_renderVector) {
			if (!RO->material) // == nullptr
				drawSingleColor(*RO);
			else if (Global::drawOutline)
				drawWithStencil(*RO);
			else
				draw(*RO);
		}
	}

	std::unique_ptr<FrameBuffer> m_FBODirLight{ nullptr };
	std::unique_ptr<FrameBuffer> m_FBOSpotLight{ nullptr };
	std::unique_ptr<FrameBuffer> m_FBOFlashLight{ nullptr };

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