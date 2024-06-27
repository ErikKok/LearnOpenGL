#pragma once

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>



// TODO hoe weet renderer welke uniforms en ssbo's hij moet doen, zonder dit hard te coden?
// vaste elementen gebruiken voor de passes? maar wat als 1 pass vaker voorkomt (spotlight)
// die kan in een aparte vector
void Renderer::draw(const RenderObject& RO) const
{
	// TODO onderstaande checks een algemene functie van maken?
	assert(RO.mesh && "No mesh defined, is this a RenderObject for a Model?");
	assert(RO.instances == std::ssize(RO.transform) && "Amount of instances and models is not equal!");

	// Activate Shader + set material properties
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

		RO.material->shader->useShader();

		// Material, dit zijn vaste waardes, die potentieel elke keer, veranderen per draw call, dus hard coded is ok?
		RO.material->shader->setInt("material.diffuse1", RO.material->diffuse1);
		RO.material->shader->setInt("material.specular1", RO.material->specular1);
		//RO.material->shader->setInt("material.normal1", RO.material->normal1);
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

	RO.mesh->m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);

	if (!RO.material->enableGL_CULL_FACE)
		glEnable(GL_CULL_FACE);

	if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight1 || m_renderPassActive == renderPassType::depthMapSpotLight0) {
		//glCullFace(GL_BACK);
	}

	Global::glCheckError();
	//std::println("RENDERER draw");
};

void Renderer::drawModel(const RenderObject& RO, Model& model) // TODO const const?
{
	// This is basically the same function as normal draw(), but the samplers are set from model.m_textures instead of Material
	// and it ignores the meshes from the RO

	assert(!RO.mesh && "RenderObject contains a Mesh, is this a RenderObject for a Mesh instead of a Model?");

	// Activate Shader + set material properties
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

		RO.material->shader->useShader();

		// Material, dit zijn vaste waardes, die potentieel elke keer, veranderen per draw call, dus hard coded is ok?
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

	// Bind all unique textures to a texture unit, so they are ready to use
	// Using TU 16 to 31 (always starting from 16, so only one model can be loaded at once -> TODO)
	for (unsigned int i{ 0u }; i < model.m_texturesLoaded.size(); i++)
	{
		assert(model.m_texturesLoaded.size() >= 1 && "Model contains no textures!");
		assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
		if (model.m_texturesLoaded[i]->getBound() == -1) {
			// activate proper texture unit (i) and bind texture
			model.m_texturesLoaded[i]->bind(i + 16);
		}
	}

	// Set material sampler2D uniforms to the correct texture unit for each texture in the Mesh
	unsigned int diffuseCount{ 1u };
	unsigned int specularCount{ 1u };
	unsigned int normalCount{ 1u }; // TODO
	//unsigned int heightCount{ 1u }; // TODO

	for (auto i{ 0 }; i < std::ssize(model.m_meshes[i].m_textures); i++)
	{
		assert(model.m_meshes[i].m_textures[i]->getBound() >= 0 && "Texture is not bound to a texture unit");

		// retrieve texture number (the N in <typename>N)
		std::string count{};
		textureType textureType{ model.m_meshes[i].m_textures[i]->getType() };
		if (textureType == textureType::diffuse)
			count = std::to_string(diffuseCount++); // transfer unsigned int to string
		else if (textureType == textureType::specular)
			count = std::to_string(specularCount++);
		else if (textureType == textureType::normal)
			count = std::to_string(normalCount++);
		//else if (textureType == textureType::height) // TODO
			//count = std::to_string(heightCount++);

		std::string result{ "material." + model.m_meshes[i].m_textures[i]->getTypeAsString() + count };
		if (m_renderPassActive == renderPassType::normal) {
			RO.material->shader->setInt(result, model.m_meshes[i].m_textures[i]->getBound());
		}
	}

	for (auto i{ 0 }; i < std::ssize(model.m_meshes); i++)
	{
		model.m_meshes[i].m_vao->bindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES, model.m_meshes[i].m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
	}

	if (!RO.material->enableGL_CULL_FACE)
		glEnable(GL_CULL_FACE);

	//// You could unbind after each call, so you can call this function for a second model... quick fix
	//for (auto i{ 0 }; i < std::ssize(m_texturesLoaded); i++)
	//{
	//    assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
	//    if (m_texturesLoaded[i]->getBound() != -1) {
	//        m_texturesLoaded[i]->unbindTexture();
	//    }
	//}

	Global::glCheckError();
}

void Renderer::drawSingleColor(const RenderObject& RO) const
{
	m_shaderSingleColor->useShader();

	for (auto i = 0; i < std::ssize(RO.ssbo); i++) {
		if (RO.ssbo[i]->getType() == SSBO::singleColor || RO.ssbo[i]->getType() == SSBO::MVP)
			RO.ssbo[i]->bind();
	}

	// TODO added a check for nullptr, workaround for lightCube which does not have a material, i guess it should have one...
	if (RO.material && !RO.material->enableGL_CULL_FACE)
		glDisable(GL_CULL_FACE);

	RO.mesh->m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);

	if (RO.material && !RO.material->enableGL_CULL_FACE)
		glEnable(GL_CULL_FACE);

	Global::glCheckError();
};

void Renderer::goRenderSkybox(const Mesh& mesh) const {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	m_shaderSkybox->useShader();
	m_shaderSkybox->setMat4("viewProjectionMatrixTranslationRemoved", Global::camera.getProjectionMatrix() * glm::mat4(glm::mat3(Global::camera.getViewMatrix()))); // remove translation from the view matrix (cast to mat3 and back to mat4)
	mesh.m_vao->bindVertexArray();
	glCullFace(GL_FRONT); // TODO cube is viewed from the inside, however there is a simple correction, reverse the order of vertices, and it will become front-facing-outward (not inward). klopt dat? of zo laten...?
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Renderer::drawFrustum(const Mesh& mesh, const glm::mat4& lightViewProjectionMatrix) const
{
	m_shaderFrustum->useShader();
	m_shaderFrustum->setMat4("viewProjectionMatrix", Global::camera.getViewProjectionMatrix());
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

	Global::glCheckError();
};

void Renderer::drawDebugQuad(const Mesh& mesh, const Camera& useCamera) const
{
	m_shaderDebugQuad->useShader();
	m_shaderDebugQuad->setMat4("model", Global::calculateModelMatrix(glm::vec3(0.6f, 0.6f, -1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.0f)));
	m_shaderDebugQuad->setBool("orthographic", useCamera.getOrthographic());
	m_shaderDebugQuad->setFloat("nearPlane", useCamera.getNearPlane());
	m_shaderDebugQuad->setFloat("farPlane", useCamera.getFarPlane());

	mesh.m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);

	Global::glCheckError();
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

void Renderer::goRenderOutline() {
	if (Global::drawOutline) { // TODO make framerate independent and independent of this function
		if (Global::outlineAlpha >= 0.0f)
			Global::outlineAlpha += 0.01f;
		if (Global::outlineAlpha >= 1.0f)
			Global::outlineAlpha = 0.0f;
		glm::vec4 color{ 1.0f, 0.28f, 0.26f, Global::outlineAlpha }; // TODO get color from SSBO

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw according to stencil buffer
		for (auto& RO : m_renderVector) {
			if (RO->drawOutline) {
				for (auto i = 0; i < std::ssize(RO->ssbo); i++) {
					if (RO->ssbo[i]->getType() == SSBO::MVP)
						RO->ssbo[i]->updateFully(Global::camera.getViewProjectionMatrix() * glm::scale(RO->transform[0], glm::vec3(1.05f, 1.05f, 0.0f)), true);
					if (RO->ssbo[i]->getType() == SSBO::singleColor)
						RO->ssbo[i]->updateFully(color, true);
				}
				drawSingleColor(*RO);
			}
		}
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // De-init Stencil Buffer, all fragments should pass the stencil test again
	}
}

void Renderer::goRender() {
	clearStencilBuffer();

	glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way

	m_renderPassActive = renderPassType::depthMap;
	m_shaderDepthMap->useShader();
	for (auto& FBO : m_FBO) {
		assert(FBO && "FBO is nullptr");
		assert(FBO->getType() == framebufferType::depthMap && "Wrong framebufferType");

		m_renderPassActive = static_cast<renderPassType>(m_renderPassActive + 1); // TODO dit is niet nodig op het moment met uberSSBO

		if (m_renderPassActive == depthMapSpotLight0 && !SpotLight::spotLights[0].getOn() ||
			m_renderPassActive == depthMapSpotLight1 && !SpotLight::spotLights[1].getOn()   )
			continue;

		FBO->bind();
		setViewPort(FBO.get());
		clearDepthBuffer();

		for (const auto& RO : m_renderVector) {
			if (RO->drawShadow && RO->model)
				drawModel(*RO, *RO->model);
			else if (RO->drawShadow)
				draw(*RO);
		}

		FBO->unbind();
	}

	m_renderPassActive = renderPassType::normal;
	glCullFace(GL_BACK);

	setViewPort();
	clearColorAndDepthBuffer();

	for (const auto& RO : m_renderVector) {
		if (!RO->material) // == nullptr TODO should be a flag in RO
			drawSingleColor(*RO);
		else if (RO->model)
			drawModel(*RO, *RO->model);
		else if (Global::drawOutline)
			drawWithStencil(*RO);
		else
			draw(*RO);
	}
}

//void Renderer::goRender() {
//	clearStencilBuffer();
//
//	glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a shadow this way // TODO to renderer
//
//
//	m_renderPassActive = renderPassType::depthMapDirLight;
//	assert(m_FBODirLight && "FBO is nullptr");
//	assert(m_FBODirLight->getType() == framebufferType::depthMap && "Wrong framebufferType");
//	assert(m_FBODirLight->getTexture()->getType() == textureType::depthMap && "Texture has wrong Type for this constructor"); // TODO test
//
//	m_FBODirLight->bind();
//	setViewPort(m_FBODirLight.get());
//	clearDepthBuffer();
//	m_shaderDepthMapDirLight->useShader();
//
//	for (const auto& RO : m_renderVector) {
//		if (RO->drawShadow && RO->model)
//			drawModel(*RO, *RO->model);
//		else if (RO->drawShadow)
//			draw(*RO);
//	}
//
//	m_FBODirLight->unbind();
//
//	//////////////
//
//	m_renderPassActive = renderPassType::depthMapFlashLight;
//	assert(m_FBOSpotLight0 && "FBO is nullptr");
//	assert(m_FBOSpotLight0->getType() == framebufferType::depthMap && "Wrong framebufferType");
//
//	if (SpotLight::spotLights[0].getOn()) {
//		m_FBOSpotLight0->bind();
//		setViewPort(m_FBOSpotLight0.get());
//		clearDepthBuffer();
//		m_shaderDepthMapSpotLight0->useShader();
//
//		for (const auto& RO : m_renderVector) {
//			if (RO->drawShadow && RO->model)
//				drawModel(*RO, *RO->model);
//			else if (RO->drawShadow)
//				draw(*RO);
//		}
//
//		m_FBOSpotLight0->unbind();
//	}
//
//	//////////////
//
//	m_renderPassActive = renderPassType::depthMapSpotLight;
//	assert(m_FBOSpotLight1 && "FBO is nullptr");
//	assert(m_FBOSpotLight1->getType() == framebufferType::depthMap && "Wrong framebufferType");
//
//	m_FBOSpotLight1->bind();
//	setViewPort(m_FBOSpotLight1.get());
//	clearDepthBuffer();
//	m_shaderDepthMapSpotLight1->useShader();
//
//	for (const auto& RO : m_renderVector) {
//		if (RO->drawShadow && RO->model)
//			drawModel(*RO, *RO->model);
//		else if (RO->drawShadow)
//			draw(*RO);
//	}
//
//	m_FBOSpotLight1->unbind();
//
//	//////////////
//
//	m_renderPassActive = renderPassType::normal;
//	glCullFace(GL_BACK);
//
//	setViewPort();
//	clearColorAndDepthBuffer();
//
//	for (const auto& RO : m_renderVector) {
//		if (!RO->material) // == nullptr
//			drawSingleColor(*RO);
//		else if (RO->model)
//			drawModel(*RO, *RO->model);
//		else if (Global::drawOutline)
//			drawWithStencil(*RO);
//		else
//			draw(*RO);
//	}
//}

//void Renderer::drawXYZ(ShaderStorageBuffer& ssbo) const {
//
//	static constexpr std::array x{
//		   -10.0f,   0.0f,  0.0f,
//			10.0f,   0.0f,  0.0f,
//	};
//	static constexpr std::array y{
//			0.0f,   -10.0f, 0.0f,
//			0.0f,    10.0f, 0.0f,
//	};
//	static constexpr std::array z{
//			0.0f,    0.0f, -10.0f,
//			0.0f,    0.0f,  10.0f,
//	};
//
//	m_shaderSingleColor->useShader();
//	ssbo.setVectorAndUpdateAndBind(Global::camera.getProjectionMatrix() * Global::getModelViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
//
//	VertexArray xVAO;
//	VertexBuffer xVBO(sizeof(x), &x);
//	VertexAttributeLayout xLayout;
//	xLayout.pushVertexAttributeLayout<float>(3);
//	xVAO.addVertexAttributeLayout(xVBO, xLayout);
//
//	VertexArray yVAO;
//	VertexBuffer yVBO(sizeof(y), &y);
//	VertexAttributeLayout yLayout;
//	yLayout.pushVertexAttributeLayout<float>(3);
//	yVAO.addVertexAttributeLayout(yVBO, yLayout);
//
//	VertexArray zVAO;
//	VertexBuffer zVBO(sizeof(z), &z);
//	VertexAttributeLayout zLayout;
//	zLayout.pushVertexAttributeLayout<float>(3);
//	zVAO.addVertexAttributeLayout(zVBO, zLayout);
//
//	// I could separate this in 2 functions, an Init and a Draw
//
//	xVAO.bindVertexArray();
//	m_shaderSingleColor->setVec4("color", { 1.0f, 0.0f, 0.0f, 1.0f });
//	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(x.size()));
//	yVAO.bindVertexArray();
//	m_shaderSingleColor->setVec4("color", { 0.0f, 1.0f, 0.0f, 1.0f });
//	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(y.size()));
//	zVAO.bindVertexArray();
//	m_shaderSingleColor->setVec4("color", { 0.0f, 0.0f, 1.0f, 1.0f });
//	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(z.size()));
//
//	//constexpr std::array xyz{
//	//	// Positions             // Colors        
//	//	-10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
//	//	 10.0f,   0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
//	//	 0.0f,  -10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
//	//	 0.0f,   10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
//	//	 0.0f,    0.0f, -10.0f,  0.0f, 0.0f, 1.0f,
//	//	 0.0f,    0.0f,  10.0f,  0.0f, 0.0f, 1.0f,
//	//};
//}

//void Renderer::draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances) const
//{
//	if (m_renderPassActive == renderPassType::normal) {
//		material.shader.useShader();
//
//		material.shader.setInt("material.diffuse1", material.diffuse1);
//		material.shader.setInt("material.specular1", material.specular1);
//		material.shader.setInt("material.emission", material.emission);
//		material.shader.setFloat("material.emissionStrength", material.emissionStrength);
//		material.shader.setFloat("material.shininess", material.shininess);
//		material.shader.setInt("material.flashLightEmissionMap", material.flashLightEmissionMap);
//		material.shader.setInt("material.flashLightEmissionTexture", material.flashLightEmissionTexture);
//	}
//
//	if (m_renderPassActive == renderPassType::depthMapDirLight) {
//		m_shaderDepthMapDirLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//	}
//
//	if (m_renderPassActive == renderPassType::depthMapSpotLight) {
//		m_shaderDepthMapSpotLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//	}
//
//	if (m_renderPassActive == renderPassType::depthMapFlashLight) {
//		m_shaderDepthMapFlashLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//	}
//
//	vao.bindVertexArray();
//	glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances);
//
//	if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight || m_renderPassActive == renderPassType::depthMapFlashLight) {
//		//glCullFace(GL_BACK);
//	}
//
//	Global::glCheckError();
//	//std::println("RENDERER draw");
//};

//#pragma warning( suppress : 4100 )
//void Renderer::drawSingleColor(const Mesh& mesh, const glm::vec4 color, GLsizei instances) const
//{
//	m_shaderSingleColor->useShader();
//	m_shaderSingleColor->setVec4("color", color);
//	mesh.m_vao->bindVertexArray();
//	glVertexArrayVertexBuffer(mesh.m_vao->getId(), 0, mesh.m_vbo->getId(), 0, mesh.m_layout->getStride());
//	glVertexArrayElementBuffer(mesh.m_vao->getId(), mesh.m_ebo->getId());
//	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, instances);
//
//	Global::glCheckError();
//};

//void Renderer::draw(const Mesh& mesh, const Material& material, GLsizei instances) const
//{
//	// Activate Shader + set material properties
//	switch (m_renderPassActive)
//	{
//	case renderPassType::undefined:
//		assert(false); // should never be the case 
//		break;
//	case renderPassType::normal:
//		material.shader.useShader();
//
//		material.shader.setInt("material.diffuse1", material.diffuse1);
//		material.shader.setInt("material.specular1", material.specular1);
//		//material.shader.setInt("material.normal1", material.normal1);
//		material.shader.setInt("material.emission", material.emission);
//		material.shader.setFloat("material.emissionStrength", material.emissionStrength);
//		material.shader.setFloat("material.shininess", material.shininess);
//		material.shader.setInt("material.flashLightEmissionMap", material.flashLightEmissionMap);
//		material.shader.setInt("material.flashLightEmissionTexture", material.flashLightEmissionTexture);
//		break;
//	case renderPassType::depthMapDirLight:
//		m_shaderDepthMapDirLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//		break;
//	case renderPassType::depthMapSpotLight:
//		m_shaderDepthMapSpotLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//		break;
//	case renderPassType::depthMapFlashLight:
//		m_shaderDepthMapFlashLight->useShader();
//		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
//		break;
//	}
//	
//	mesh.m_vao->bindVertexArray();
//	glVertexArrayVertexBuffer(mesh.m_vao->getId(), 0, mesh.m_vbo->getId(), 0, mesh.m_layout->getStride());
//	glVertexArrayElementBuffer(mesh.m_vao->getId(), mesh.m_ebo->getId());
//	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, instances);
//
//	if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight || m_renderPassActive == renderPassType::depthMapFlashLight) {
//		//glCullFace(GL_BACK);
//	}
//
//	Global::glCheckError();
//	//std::println("RENDERER draw");
//};