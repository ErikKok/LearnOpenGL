#pragma once
#include "Renderer.h"

#include "Camera.h"
#include "FrameBuffer.h"
#include "Global.h"
#include "Light.h"
#include "Model.h"
#include "Shader.h"
#include "ShaderStorageBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

void Renderer::draw(const RenderObject& RO) const
{
	assert(RO.instances == std::ssize(RO.transform) && "Amount of instances and models is not equal!");

	switch (m_renderPassActive)
	{
	case renderPassType::undefined:
		assert(false);
		break;
	case renderPassType::depthMapDirLight:
		for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
			if (RO.ssbo[i]->getType() == SSBO::dirLightMVP)
				RO.ssbo[i]->bindOverrideBindingPoint(+SSBO::genericDepthMap);
		}
		break;
	case renderPassType::depthMapSpotLight0:
		for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
			if (RO.ssbo[i]->getType() == SSBO::spotLight0MVP)
				RO.ssbo[i]->bindOverrideBindingPoint(+SSBO::genericDepthMap);
		}
		break;
	case renderPassType::depthMapSpotLight1:
		for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
			if (RO.ssbo[i]->getType() == SSBO::spotLight1MVP)
				RO.ssbo[i]->bindOverrideBindingPoint(+SSBO::genericDepthMap);
		}
		break;
	case renderPassType::normal:
		if (!RO.material->enableGL_CULL_FACE)
			glDisable(GL_CULL_FACE);

		// Activate Shader + set material properties
		RO.material->shader->useShader();

		if (!RO.model) {
			RO.material->shader->setInt("material.diffuse1", RO.material->diffuse1);
			RO.material->shader->setInt("material.specular1", RO.material->specular1);
			//RO.material->shader->setInt("material.normal1", RO.material->normal1);
		}
		RO.material->shader->setInt("material.emissionTexture", RO.material->emissionTexture);
		RO.material->shader->setFloat("material.emissionStrength", RO.material->emissionStrength);
		RO.material->shader->setFloat("material.shininess", RO.material->shininess);
		RO.material->shader->setInt("material.lightEmissionMap", RO.material->lightEmissionMap);
		RO.material->shader->setInt("material.lightEmissionTexture", RO.material->lightEmissionTexture);

		// SSBO
		for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
			RO.ssbo[i]->bind();
		}
		break;
	}

	if (!RO.model) {
		RO.mesh->m_vao->bindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);
	}

	if (RO.model) {
		// Bind all unique textures to a texture unit, so they are ready to use
		// Using TU 16 to 31 (always starting from 16, so only one model can be loaded at once -> TODO)
		for (unsigned int i{ 0u }; i < RO.model->m_texturesLoaded.size(); i++)
		{
			assert(RO.model->m_texturesLoaded.size() >= 1 && "Model contains no textures!");
			assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
			if (RO.model->m_texturesLoaded[i]->getBound() == -1) {
				// activate proper texture unit (i) and bind texture
				RO.model->m_texturesLoaded[i]->bind(i + 16);
			}
		}

		// Set material sampler2D uniforms to the correct texture unit for each texture in the Mesh
		unsigned int diffuseCount{ 1u };
		unsigned int specularCount{ 1u };
		unsigned int normalCount{ 1u }; // TODO
		//unsigned int heightCount{ 1u }; // TODO

		for (auto i{ 0 }; i < std::ssize(RO.model->m_meshes[i].m_textures); i++)
		{
			assert(RO.model->m_meshes[i].m_textures[i]->getBound() >= 0 && "Texture is not bound to a texture unit");

			// retrieve texture number (the N in <typename>N)
			std::string count{};
			textureType textureType{ RO.model->m_meshes[i].m_textures[i]->getType() };
			if (textureType == textureType::diffuse)
				count = std::to_string(diffuseCount++); // transfer unsigned int to string
			else if (textureType == textureType::specular)
				count = std::to_string(specularCount++);
			else if (textureType == textureType::normal)
				count = std::to_string(normalCount++);
			//else if (textureType == textureType::height) // TODO
				//count = std::to_string(heightCount++);

			std::string result{ "material." + RO.model->m_meshes[i].m_textures[i]->getTypeAsString() + count };
			if (m_renderPassActive == renderPassType::normal) {
				RO.material->shader->setInt(result, RO.model->m_meshes[i].m_textures[i]->getBound());
			}
		}

		for (auto i{ 0 }; i < std::ssize(RO.model->m_meshes); i++)
		{
			RO.model->m_meshes[i].m_vao->bindVertexArray();
			glDrawElementsInstanced(GL_TRIANGLES, RO.model->m_meshes[i].m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
		}
	}

	if (!RO.material->enableGL_CULL_FACE)
		glEnable(GL_CULL_FACE);

	//if (RO.model) {
		// You could unbind after each call, so you can call this function for a second model... quick fix
		//for (auto i{ 0 }; i < std::ssize(m_texturesLoaded); i++)
		//{
		//    assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
		//    if (m_texturesLoaded[i]->getBound() != -1) {
		//        m_texturesLoaded[i]->unbindTexture();
		//    }
		//}
	//}

	G::glCheckError();
	//std::println("RENDERER draw");
};

void Renderer::drawSingleColor(const RenderObject& RO) const
{
	m_shaderSingleColor->useShader();

	for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
		if (RO.ssbo[i]->getType() == SSBO::singleColor || RO.ssbo[i]->getType() == SSBO::MVP)
			RO.ssbo[i]->bind();
	}

	if (RO.material && !RO.material->enableGL_CULL_FACE)
		glDisable(GL_CULL_FACE);

	RO.mesh->m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);

	if (RO.material && !RO.material->enableGL_CULL_FACE)
		glEnable(GL_CULL_FACE);

	G::glCheckError();
};

void Renderer::goRenderSkybox(const Mesh& mesh) const {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	m_shaderSkybox->useShader();
	m_shaderSkybox->setMat4("viewProjectionMatrixTranslationRemoved", G::camera->getProjectionMatrix() * glm::mat4(glm::mat3(G::camera->getViewMatrix()))); // remove translation from the view matrix (cast to mat3 and back to mat4)
	mesh.m_vao->bindVertexArray();
	glCullFace(GL_FRONT); // This is because cube is viewed from the inside. There is a simple correction, reverse the order of vertices, and it will become front-facing-outward (not inward). Not really needed...
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Renderer::drawFrustum(const Mesh& mesh, const glm::mat4& lightViewProjectionMatrix) const
{
	m_shaderFrustum->useShader();
	m_shaderFrustum->setMat4("viewProjectionMatrix", G::camera->getViewProjectionMatrix());
	m_shaderFrustum->setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
	m_shaderFrustum->setMat4("inverseMatrix", glm::inverse(lightViewProjectionMatrix));
	mesh.m_vao->bindVertexArray();

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_shaderFrustum->setVec4("color", { 1.0f, 0.0f, 0.0f, 0.1f });
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
	glEnable(GL_CULL_FACE);

	G::glCheckError();
};

void Renderer::drawDebugQuad(const Mesh& mesh, const Camera& camera) const
{
	// correct quad for the aspectRatio of the depthmap and viewport
	float height{ 0.3f };
	float width = (height / (static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight))) * camera.getAspectRatio();

	// correct margin for viewport
	float margin{ 0.05f };
	float translateX{ 1.0f - (width + (margin / (static_cast<float>(G::windowWidth) / static_cast<float>(G::windowHeight)))) };
	float translateY{ 1.0f - (height + margin) };
	
	m_shaderDebugQuad->useShader();
	m_shaderDebugQuad->setMat4("model", G::calculateModelMatrix(glm::vec3(translateX, translateY, 0.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(width, height, 0.0f)));
	m_shaderDebugQuad->setBool("orthographic", camera.getOrthographic());
	m_shaderDebugQuad->setFloat("nearPlane", camera.getNearPlane());
	m_shaderDebugQuad->setFloat("farPlane", camera.getFarPlane());

	mesh.m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);

	G::glCheckError();
};

void Renderer::clearStencilBuffer() const {
	glStencilMask(0xFF); // enable writing to the stencil buffer, so it can be cleared
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00); // disable writing to the stencil buffer (until needed)
}

void Renderer::initStencilBuffer() {
	glEnable(GL_STENCIL_TEST);
	// if both stencil test and depth test succeed replace stencil value
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// all fragments should pass the stencil test until we need to draw a stencil
	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	// some information:
	// to actually change the depth clearing value, you need to call glClearDepth() before calling glClear()
	// 
	// https://a.disquscdn.com/uploads/mediaembed/images/2106/4734/original.jpg
	// glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
	// glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Renderer::drawWithStencil(const RenderObject& RO) {
	glStencilMask(0xFF);
	draw(RO);
	glStencilMask(0x00);
}

void Renderer::goRender() {
	clearStencilBuffer();

		glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way

		m_renderPassActive = renderPassType::depthMap;
		m_shaderDepthMap->useShader();
		for (auto& FBO : m_FBO) {
			assert(FBO && "FBO is nullptr");
			assert(FBO->getType() == framebufferType::depthMap && "Wrong framebufferType");

			m_renderPassActive = static_cast<renderPassType>(m_renderPassActive + 1);

			if (m_renderPassActive == depthMapSpotLight0 && !SpotLight::spotLights[0].getOn() ||
				m_renderPassActive == depthMapSpotLight1 && !SpotLight::spotLights[1].getOn())
				continue;

			FBO->bind();
			setViewPort(FBO.get());
			clearDepthBuffer();

			for (const auto& RO : m_renderVector) {
				if (RO->castsShadow)
					draw(*RO);
			}

			FBO->unbind();
		}
		glCullFace(GL_BACK);

	m_renderPassActive = renderPassType::normal;

	setViewPort();
	clearColorAndDepthBuffer();

	for (const auto& RO : m_renderVector) {
		if (RO->drawAsSingleColor)
			drawSingleColor(*RO);
		else if (G::drawOutline && RO->drawOutline)
			drawWithStencil(*RO);
		else
			draw(*RO);
	}
}

void Renderer::goRenderOutline() {
	if (G::drawOutline) {
		if (G::outlineAlpha >= 0.0f)
			G::outlineAlpha += 1.0f * G::deltaTime;
		if (G::outlineAlpha >= 1.0f)
			G::outlineAlpha = 0.0f;
		glm::vec4 color{ 1.0f, 0.28f, 0.26f, G::outlineAlpha }; // TODO get color from SSBO

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw according to stencil buffer
		for (auto& RO : m_renderVector) {
			if (RO->drawOutline) {
				for (auto i = 0; i < std::ssize(RO->ssbo); i++) {
					if (RO->ssbo[i]->getType() == SSBO::MVP)
						RO->ssbo[i]->updateFully(G::camera->getViewProjectionMatrix() * glm::scale(RO->transform[0], glm::vec3(1.05f, 1.05f, 0.0f)), true);
					if (RO->ssbo[i]->getType() == SSBO::singleColor)
						RO->ssbo[i]->updateFully(color, true);
				}
				drawSingleColor(*RO);
			}
		}
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // De-init Stencil Buffer, all fragments should pass the stencil test again
	}
}