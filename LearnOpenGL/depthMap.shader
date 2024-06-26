#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

layout(binding = 8, std430) readonly buffer lightMVPMatrixSSBO {
    mat4 lightMVPMatrix[10];
};

void main()
{
    gl_Position = lightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 