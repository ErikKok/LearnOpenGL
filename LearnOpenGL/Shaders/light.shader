#shader vertex
#version 420 core

// Just render some vertexes in a single color, without textures or lighting

layout (location = 0) in vec3 aPos;

uniform mat4 MVPMatrix;

void main()
{
    gl_Position = MVPMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}