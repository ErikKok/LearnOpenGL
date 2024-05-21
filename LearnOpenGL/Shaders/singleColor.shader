#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

// Just render some vertexes in a single color, without textures or lighting

layout(binding = 4, std430) readonly buffer ssboMVPMatrix {
    mat4 MVPMatrix[];
};

void main()
{
    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color; // TODO array van maken?

void main()
{
    FragColor = color;
}