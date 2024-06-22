#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

// Just render some vertexes in a single color, without textures or lighting

//layout(binding = 4, std430) readonly buffer ssboMVPMatrix {
//    mat4 MVPMatrix[];
//};

layout(binding = 24, std430) readonly buffer uberSSBO {
    mat4 dirLightMVPMatrix[10];
    mat4 flashLightMVPMatrix[10];
    mat4 spotLightMVPMatrix[10];
    mat4 normalMatrix[10];
    mat4 modelViewMatrix[10];
    mat4 MVPMatrix[10];
};

layout(binding = 20, std430) readonly buffer colorSSBO {
    vec4 color[];
};

out vec4 colorInstanced;

void main()
{
    colorInstanced = color[gl_InstanceID];
    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 colorInstanced;

void main()
{
    FragColor = colorInstanced;
}