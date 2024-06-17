#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

//layout(binding = 6, std430) readonly buffer spotLightMVPMatrixSSBO {
//    mat4 spotLightMVPMatrix[];
//};

layout(binding = 24, std430) readonly buffer uberSSBO {
    mat4 dirLightMVPMatrix[10];
    mat4 flashLightMVPMatrix[10];
    mat4 spotLightMVPMatrix[10];
    mat4 normalMatrix[10];
    mat4 modelViewMatrix[10];
    mat4 MVPMatrix[10];
};

void main()
{
    gl_Position = spotLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0);
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 