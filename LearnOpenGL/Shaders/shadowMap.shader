#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

uniform bool orthographic;

layout(binding = 5, std430) readonly buffer LightOrthoMVPMatrixSSBO {
    mat4 LightOrthoMVPMatrix[];
};

layout(binding = 6, std430) readonly buffer LightPerspectiveMVPMatrixSSBO {
    mat4 LightPerspectiveMVPMatrix[];
};

void main()
{
    if (orthographic) {
        gl_Position = LightOrthoMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    }
    else if (!orthographic) {
        gl_Position = LightPerspectiveMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    }
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 