#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

layout(binding = 5, std430) readonly buffer ssboLightMVPMatrix {
    mat4 LightMVPMatrix[];
};
void main()
{
    gl_Position = LightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0);
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 