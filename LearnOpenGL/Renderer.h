#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Shader.h"
#include "VertexArray.h"

#include <memory> // for std::unique_ptr

// TODO transparency opslaan in Material, of uit texture halen in constructor van een Material class
struct Material {
	Shader& shader;
	int diffuse1{ 0 };				// sampler2D
	int specular1{ 0 };				// sampler2D
	//int normal1;					// sampler2D
	//int height1;					// sampler2D
	int emission{ 0 };				// sampler2D
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

	const Shader* getShaderShadowMapDirLight() const { return m_shaderShadowMapDirLight.get(); };
	void createShaderShadowMapDirLight(std::string string) { m_shaderShadowMapDirLight = std::make_unique<Shader>(string); };
	const Shader* getShaderShadowMapSpotLight() const { return m_shaderShadowMapSpotLight.get(); };
	void createShaderShadowMapSpotLight(std::string string) { m_shaderShadowMapSpotLight = std::make_unique<Shader>(string); };
	const Shader* getShaderShadowMapFlashLight() const { return m_shaderShadowMapFlashLight.get(); };
	void createShaderShadowMapFlashLight(std::string string) { m_shaderShadowMapFlashLight = std::make_unique<Shader>(string); };
	const Shader* getShaderSingleColor() const { return m_shaderSingleColor.get(); };
	void createShaderSingleColor(std::string string) { m_shaderSingleColor = std::make_unique<Shader>(string); };
	void createShaderSkybox(std::string string) { m_shaderSkybox = std::make_unique<Shader>(string); };

	void clear()
	{
		//glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // magenta
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	// TODO store the vao/ebo's etc in a list/batch/whatever, order them, then batch render them
	// void draw(RederBatch);
	void draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances = 1, bool triangleStrip = false) const
	{
		if (m_renderPassActive == renderPassType::normal) {
			material.shader.useShader();

			material.shader.setInt("material.diffuse1", material.diffuse1);
			material.shader.setInt("material.specular1", material.specular1);
			material.shader.setInt("material.emission", material.emission);
			material.shader.setFloat("material.emissionStrength", material.emissionStrength);
			material.shader.setFloat("material.shininess", material.shininess);
			material.shader.setInt("material.flashLightEmissionMap", material.flashLightEmissionMap);
			material.shader.setInt("material.flashLightEmissionTexture", material.flashLightEmissionTexture);
		}

		if (m_renderPassActive == renderPassType::depthMapDirLight) {
			m_shaderShadowMapDirLight->useShader();
			//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way
		}

		if (m_renderPassActive == renderPassType::depthMapSpotLight) {
			m_shaderShadowMapSpotLight->useShader();
			//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way
		}

		if (m_renderPassActive == renderPassType::depthMapFlashLight) {
			m_shaderShadowMapFlashLight->useShader();
			//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way
		}

		vao.bindVertexArray();
		if (triangleStrip) // TODO dit uit de mesh halen, die uiteindelijk als invoer moet dienen voor deze functie
			glDrawElementsInstanced(GL_TRIANGLE_STRIP, ebo.getCount(), GL_UNSIGNED_INT, 0, instances); // ebo.getCount() untested!
		if (!triangleStrip)
			glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances); // ebo.getCount() untested!

		if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight || m_renderPassActive == renderPassType::depthMapFlashLight) {
			//glCullFace(GL_BACK);
		}

		Global::glCheckError();
		//std::println("RENDERER draw");
	};

	void drawSingleColor(const VertexArray& vao, const ElementBuffer& ebo, const glm::vec4 color, GLsizei instances = 1, bool triangleStrip = false) const
	{
		m_shaderSingleColor->useShader();
		m_shaderSingleColor->setVec4("color", color);
		vao.bindVertexArray();
		if (triangleStrip) // TODO dit uit de mesh halen, die uiteindelijk als invoer moet dienen voor deze functie
			glDrawElementsInstanced(GL_TRIANGLE_STRIP, ebo.getCount(), GL_UNSIGNED_INT, 0, instances);
		if (!triangleStrip)
			glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances);

		Global::glCheckError();
	};

	void drawXYZ(ShaderStorageBuffer& ssbo) {

		static constexpr std::array x{  
			-10.0f,   0.0f,  0.0f,
			 10.0f,   0.0f,  0.0f,
		};
		static constexpr std::array y{
			 0.0f,   -10.0f, 0.0f,
			 0.0f,    10.0f, 0.0f,
		};
		static constexpr std::array z{
			 0.0f,    0.0f, -10.0f,
			 0.0f,    0.0f,  10.0f,
		};

		m_shaderSingleColor->useShader();
		ssbo.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

		VertexArray xVAO;
		VertexBuffer xVBO(sizeof(x), &x);
		VertexAttributeLayout xLayout;
		xLayout.pushVertexAttributeLayout<float>(3);
		xVAO.addVertexAttributeLayout(xVBO, xLayout);

		VertexArray yVAO;
		VertexBuffer yVBO(sizeof(y), &y);
		VertexAttributeLayout yLayout;
		yLayout.pushVertexAttributeLayout<float>(3);
		yVAO.addVertexAttributeLayout(yVBO, yLayout);

		VertexArray zVAO;
		VertexBuffer zVBO(sizeof(z), &z);
		VertexAttributeLayout zLayout;
		zLayout.pushVertexAttributeLayout<float>(3);
		zVAO.addVertexAttributeLayout(zVBO, zLayout);

		// I could separate this in 2 functions, an Init and a Draw

		xVAO.bindVertexArray();
		m_shaderSingleColor->setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(x.size()));
		yVAO.bindVertexArray();
		m_shaderSingleColor->setVec4("color", { 0.0f, 1.0f, 0.0f, 1.0f });
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(y.size()));
		zVAO.bindVertexArray();
		m_shaderSingleColor->setVec4("color", { 0.0f, 0.0f, 1.0f, 1.0f });
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(z.size()));

		//constexpr std::array xyz{
		//	// Positions             // Colors        
		//	-10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
		//	 10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
		//	 0.0f,  -10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
		//	 0.0f,   10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
		//	 0.0f,    0.0f, -10.0f,  0.0f, 0.0f, 1.0f,
		//	 0.0f,    0.0f,  10.0f,  0.0f, 0.0f, 1.0f,
		//};
	}

	void drawSkybox(const VertexArray& vao, const ElementBuffer& ebo) {
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		m_shaderSkybox->useShader();
		m_shaderSkybox->setMat4("viewProjectionMatrixTranslationRemoved", Global::camera.getProjectionMatrix() * glm::mat4(glm::mat3(Global::camera.getViewMatrix()))); // remove translation from the view matrix (cast to mat3 and back to mat4)
		vao.bindVertexArray();
		//cubemapTexture.bindTexture(); // No need to bind if there is just a single GL_TEXTURE_CUBE_MAP
		glCullFace(GL_FRONT); // TODO cube is viewed from the inside, however there is a simple correction, reverse the order of vertices, and it will become front-facing-outward (not inward). klopt dat? of zo laten...?
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, ebo.getCount(), GL_UNSIGNED_INT, 0, 1);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

private:
	renderPassType m_renderPassActive{ renderPassType::undefined };

	std::unique_ptr<Shader> m_shaderShadowMapDirLight{ nullptr }; //before we use either of these operators, we should check whether the std::unique_ptr actually has a resource
	std::unique_ptr<Shader> m_shaderShadowMapSpotLight{ nullptr };
	std::unique_ptr<Shader> m_shaderShadowMapFlashLight{ nullptr };
	std::unique_ptr<Shader> m_shaderSingleColor{ nullptr };
	std::unique_ptr<Shader> m_shaderSkybox{ nullptr };

	bool glStencilFuncActive{};
	bool glDepthTestActive{}; // ?

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_FRAMEBUFFER_SRGB);
};