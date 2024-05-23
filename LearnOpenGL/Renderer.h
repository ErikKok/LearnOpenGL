#pragma once

#include "Buffers.h"
#include "Global.h"
#include "Shader.h"
//#include "Texture.h"
#include "VertexArray.h"

struct Material {
	Shader& shader;
	Shader& shaderShadowMapDirLight;
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
	normal,
	depthMapDirLight,
	depthMapSpotLight,
	depthMapFlashLight,
};

class Renderer {
public:
	//static inline renderPassType s_renderPassActive{ renderPassType::normal };

	// TODO private
	//renderPassType s_renderPassActive;
	//Shader sshaderShadowMapDirLight; // Shader&
	//static std::unique_ptr<Shader> sshaderShadowMapSpotLight;
	//static std::unique_ptr<Shader> sshaderShadowMapFlashLight;

	void clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	void draw(const VertexArray& vao, const ElementBuffer& ebo, const Material& material, GLsizei instances = 1) const
	{
		if (!FrameBuffer::s_depthMapPassActive) {
			material.shader.useShader();

			material.shader.setInt("material.diffuse1", material.diffuse1);
			material.shader.setInt("material.specular1", material.specular1);
			material.shader.setInt("material.emission", material.emission);
			material.shader.setFloat("material.emissionStrength", material.emissionStrength);
			material.shader.setFloat("material.shininess", material.shininess);
			material.shader.setInt("material.flashLightEmissionMap", material.flashLightEmissionMap); // deze kan op nul, ipv flashLight.emissionStrength
			material.shader.setInt("material.flashLightEmissionTexture", material.flashLightEmissionTexture);
		}
		else {
			material.shaderShadowMapDirLight.useShader();

			//material.shaderShadowMapDirLight.setBool("orthographic", true);
		}

		//if (s_renderPassActive == renderPassType::normal)
		//	material.shader.useShader();



		vao.bindVertexArray();

		glDrawElementsInstanced(GL_TRIANGLES, ebo.getCount(), GL_UNSIGNED_INT, 0, instances); // ebo.getCount() untested!

		Global::glCheckError();
		//std::println("RENDERER draw");	
	};

private:

};

// TODO take in a material, instead of shader, which contains all material data + corresponding shader