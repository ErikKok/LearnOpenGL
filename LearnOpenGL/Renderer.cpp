#pragma once

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

void Renderer::clear() const
{
	//glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // magenta
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

// TODO hoe weet renderer welke uniforms en ssbo's hij moet doen, zonder dit hard te coden?
// vaste elementen gebruiken voor de passes? maar wat als 1 pass vaker voorkomt (spotlight)
// die kan in een aparte vector
void Renderer::draw(const RenderObject& RO) const
{
	// TODO onderstaande checks een algemene functie van maken?
	assert(RO.mesh && "No mesh defined, is this a RenderObject for a Model?");
	assert(RO.instances == std::ssize(RO.model) && "Amount of instances and models is not equal!");
	// TODO more checks needed?

	// Activate Shader + set material properties
	switch (m_renderPassActive)
	{
	case renderPassType::undefined:
		assert(false); // should never be the case 
		break;
	case renderPassType::depthMapDirLight:
		m_shaderDepthMapDirLight->useShader();
		RO.ssbo[0]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::depthMapFlashLight:
		m_shaderDepthMapFlashLight->useShader();
		RO.ssbo[1]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::depthMapSpotLight:
		m_shaderDepthMapSpotLight->useShader();
		RO.ssbo[2]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::normal:
		RO.material->shader.useShader();

		// Material, dit zijn vaste waardes, die potentieel elke keer, veranderen per draw call, dus hard coded is ok?
		RO.material->shader.setInt("material.diffuse1", RO.material->diffuse1);
		RO.material->shader.setInt("material.specular1", RO.material->specular1);
		//RO.material.shader.setInt("material.normal1", RO.material->normal1);
		RO.material->shader.setInt("material.emission", RO.material->emission);
		RO.material->shader.setFloat("material.emissionStrength", RO.material->emissionStrength);
		RO.material->shader.setFloat("material.shininess", RO.material->shininess);
		RO.material->shader.setInt("material.flashLightEmissionMap", RO.material->flashLightEmissionMap);
		RO.material->shader.setInt("material.flashLightEmissionTexture", RO.material->flashLightEmissionTexture);

		// SSBO
		for (int i = 0; i < std::size(RO.ssbo); i++) {
			RO.ssbo[i]->bind();
		}
		break;
	}

	RO.mesh->m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);

	if (m_renderPassActive == renderPassType::depthMapDirLight || m_renderPassActive == renderPassType::depthMapSpotLight || m_renderPassActive == renderPassType::depthMapFlashLight) {
		//glCullFace(GL_BACK);
	}

	Global::glCheckError();
	//std::println("RENDERER draw");
};

void Renderer::drawModel(const RenderObject& RO, Model& model) // TODO const const?
{
	// This is basically the same function as normal draw(), but the samplers are set from model.m_textures instead of Material
	// and it ignores the meshes from the RO

	assert(!RO.mesh && "Mesh defined, is this a RenderObject for a Mesh?");

	// Activate Shader + set material properties
	switch (m_renderPassActive)
	{
	case renderPassType::undefined:
		assert(false); // should never be the case 
		break;
	case renderPassType::depthMapDirLight:
		m_shaderDepthMapDirLight->useShader();
		RO.ssbo[0]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::depthMapFlashLight:
		m_shaderDepthMapFlashLight->useShader();
		RO.ssbo[1]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::depthMapSpotLight:
		m_shaderDepthMapSpotLight->useShader();
		RO.ssbo[2]->bind();
		//glCullFace(GL_FRONT); // use instead (or in addition to?) of bias in the shader, only draw back faces (culling front faces), but 2d faces won't cast a Depth this way
		break;
	case renderPassType::normal:
		RO.material->shader.useShader();

		// Material, dit zijn vaste waardes, die potentieel elke keer, veranderen per draw call, dus hard coded is ok?
		RO.material->shader.setInt("material.emission", RO.material->emission);
		RO.material->shader.setFloat("material.emissionStrength", RO.material->emissionStrength);
		RO.material->shader.setFloat("material.shininess", RO.material->shininess);
		RO.material->shader.setInt("material.flashLightEmissionMap", RO.material->flashLightEmissionMap);
		RO.material->shader.setInt("material.flashLightEmissionTexture", RO.material->flashLightEmissionTexture);

		// SSBO
		for (int i = 0; i < std::size(RO.ssbo); i++) {
			RO.ssbo[i]->bind();
		}
		break;
	}

	// Bind all unique textures to a texture unit, so they are ready to use
	// Using TU 16 to 31 (always starting from 16, so only one model can be loaded at once -> TODO)
	for (unsigned int i{ 0u }; i < model.m_texturesLoaded.size(); i++)
	{
		assert(i <= 15 && "Model uses > 16 textures, this is not supported!");
		if (model.m_texturesLoaded[i]->getBound() == -1) {
			// activate proper texture unit (i) and bind texture
			model.m_texturesLoaded[i]->bind(i + 16);
			// save texture unit in texture
			model.m_texturesLoaded[i]->setBound(i + 16);
		}
		//std::println("DRAW Texture bind #{}", i)
	}

	// Set material sampler2D uniforms to the correct texture unit for each texture in the Mesh
	unsigned int diffuseCount{ 1u };
	unsigned int specularCount{ 1u };
	unsigned int normalCount{ 1u }; // TODO
	//unsigned int heightCount{ 1u }; // TODO

	for (unsigned int i{ 0u }; i < model.m_meshes[i].m_textures.size(); i++)
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
			RO.material->shader.setInt(result, model.m_meshes[i].m_textures[i]->getBound());
		}
	}

	for (unsigned int i{ 0u }; i < model.m_meshes.size(); i++)
	{
		model.m_meshes[i].m_vao->bindVertexArray();
		glDrawElementsInstanced(GL_TRIANGLES, model.m_meshes[i].m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);
	}

	//// You could unbind after each call, so you can call this function for a second model... quick fix
	//for (unsigned int i{ 0u }; i < m_texturesLoaded.size(); i++)
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

	// SSBO
	for (int i = 0; i < std::size(RO.ssbo); i++) {	
		if (RO.ssbo[i]->getBindingPoint() == 4 || RO.ssbo[i]->getBindingPoint() == 20) // 4 = MVPMatrix, 20 = color
			RO.ssbo[i]->bind();
	}

	RO.mesh->m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, RO.mesh->m_ebo->getCount(), GL_UNSIGNED_INT, 0, RO.instances);

	Global::glCheckError();
};

void Renderer::drawSkybox(const Mesh& mesh) const {
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
	m_shaderDebugQuad->setMat4("model", Global::getModelMatrix(glm::vec3(0.6f, 0.6f, -1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.0f)));
	m_shaderDebugQuad->setBool("orthographic", false); // TODO get from camera class // true for dirLight only
	m_shaderDebugQuad->setFloat("nearPlane", useCamera.getNearPlane());
	m_shaderDebugQuad->setFloat("farPlane", useCamera.getFarPlane());

	mesh.m_vao->bindVertexArray();
	glDrawElementsInstanced(GL_TRIANGLES, mesh.m_ebo->getCount(), GL_UNSIGNED_INT, 0, 1);

	Global::glCheckError();
};

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