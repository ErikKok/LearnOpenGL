#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    Color = aColor;
}