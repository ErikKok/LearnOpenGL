#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

layout(binding = 2, std430) readonly buffer ssboMVPMatrix {
    mat4 MVPMatrix[];
};

void main()
{
    TexCoords = aTexCoords;
    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
}