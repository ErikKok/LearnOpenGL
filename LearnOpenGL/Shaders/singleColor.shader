#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

// Just render some vertexes in a single color, without textures or lighting

layout(binding = 5, std430) readonly buffer ssboMVPMatrix {
    mat4 MVPMatrix[];
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