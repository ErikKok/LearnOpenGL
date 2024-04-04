#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0);
    //gl_Position = projection * view * model * vec4(aPos, 1.0); // if model is in World space
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);
}