#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 MVPMatrix;

void main()
{
    gl_Position = MVPMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);
}