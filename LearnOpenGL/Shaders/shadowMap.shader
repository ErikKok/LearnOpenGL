#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

//uniform mat4 lightSpaceMatrix;
//uniform mat4 model;

//layout(binding = 5, std430) readonly buffer ssboLightModel {
//    mat4 LightModel[];
//};

layout(binding = 6, std430) readonly buffer ssboLightMVPMatrix {
    mat4 LightMVPMatrix[];
};
void main()
{
    //gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
    //gl_Position = lightSpaceMatrix * LightModel[gl_InstanceID] * vec4(aPos, 1.0); // lightMVP
    gl_Position = LightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0); // lightMVP
}  

#shader fragment
#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
} 