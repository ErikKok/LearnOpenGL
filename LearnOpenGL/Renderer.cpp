#pragma once

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Buffers.h"
//#include "Global.h"
//#include "Shader.h"
//#include "VertexArray.h"

//#include <memory> // for std::unique_ptr

void Renderer::clear() const
{
	//glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // magenta
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void Renderer::draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances) const
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
		m_shaderDepthMapDirLight->useShader();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
	}

	if (m_renderPassActive == renderPassType::depthMapSpotLight) {
		m_shaderDepthMapSpotLight->useShader();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
	}

	if (m_renderPassActive == renderPassType::depthMapFlashLight) {
		m_shaderDepthMapFlashLight->useShader();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
	}

	vao.bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances); // ebo.getCount() untested!

	if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight || m_renderPassActive == renderPassType::depthMapFlashLight) {
		//glCullFace(GL_BACK);
	}

	Global::glCheckError();
	//std::println("RENDERER draw");
};

void Renderer::drawSingleColor(const VertexArray& vao, const ElementBuffer& ebo, const glm::vec4 color, GLsizei instances) const
{
	m_shaderSingleColor->useShader();
	m_shaderSingleColor->setVec4("color", color);
	vao.bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances);

	Global::glCheckError();
};

void Renderer::drawXYZ(ShaderStorageBuffer& ssbo) const {

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

void Renderer::drawSkybox(const VertexArray& vao, const ElementBuffer& ebo) const {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	m_shaderSkybox->useShader();
	m_shaderSkybox->setMat4("viewProjectionMatrixTranslationRemoved", Global::camera.getProjectionMatrix() * glm::mat4(glm::mat3(Global::camera.getViewMatrix()))); // remove translation from the view matrix (cast to mat3 and back to mat4)
	vao.bindVertexArray();
	glCullFace(GL_FRONT); // TODO cube is viewed from the inside, however there is a simple correction, reverse the order of vertices, and it will become front-facing-outward (not inward). klopt dat? of zo laten...?
	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, 1);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS); // set depth function back to defaultsizeof(
}

void Renderer::drawFrustum(const VertexArray& vao, const ElementBuffer& ebo, const glm::mat4& lightViewProjectionMatrix) const
{
	m_shaderFrustum->useShader();
	m_shaderFrustum->setMat4("viewProjectionMatrix", Global::camera.getViewProjectionMatrix());
	m_shaderFrustum->setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
	m_shaderFrustum->setMat4("inverseMatrix", glm::inverse(lightViewProjectionMatrix));
	vao.bindVertexArray();

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_shaderFrustum->setVec4("color", { 1.0f, 0.0f, 0.0f, 0.1f });
	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, 1);
	glEnable(GL_CULL_FACE);

	Global::glCheckError();
};

void Renderer::drawDebugQuad(const VertexArray& vao, const Camera& useCamera) const
{
	m_shaderDebugQuad->useShader();
	m_shaderDebugQuad->setMat4("model", Global::getModelMatrix(glm::vec3(0.6f, 0.6f, -1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.0f)));
	m_shaderDebugQuad->setBool("orthographic", true); // TODO get from camera class
	m_shaderDebugQuad->setFloat("nearPlane", useCamera.getNearPlane());
	m_shaderDebugQuad->setFloat("farPlane", useCamera.getFarPlane());
	vao.bindVertexArray();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	Global::glCheckError();
};