#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
//uniform mat4 model;

layout(binding = 5, std430) readonly buffer ssboShadowModel {
    mat4 shadowModel[];
};

void main()
{
    //gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    gl_Position = lightSpaceMatrix * shadowModel[gl_InstanceID] * vec4(aPos, 1.0);
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 