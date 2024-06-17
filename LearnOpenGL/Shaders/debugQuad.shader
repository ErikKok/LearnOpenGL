#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;

void main()
{  
    TexCoords = aTexCoords;
    gl_Position = model * vec4(aPos, 1.0f);
} 

#shader fragment
#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

layout (binding=5) uniform sampler2D someTexture;

uniform bool orthographic;
uniform float nearPlane;
uniform float farPlane;

void main()
{ 
    float depthValue = texture(someTexture, TexCoords).r;

        // if type = depthMap {

        if (orthographic) {  // Transform non-linear depth values to linear for visibility when using perspective projection

            FragColor = vec4(vec3(depthValue), 1.0f);
        }
        else if (!orthographic) { // Orthographic projection
            depthValue = depthValue * 2.0f - 1.0f; // Back to NDC 
            depthValue = (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - depthValue * (farPlane - nearPlane)); // Linearized
            FragColor = vec4(vec3(depthValue / farPlane), 1.0f);
        }
    // }
}