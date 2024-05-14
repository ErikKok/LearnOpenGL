#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos; // vec2! not vec3 as in some other shaders
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;

void main()
{  
    TexCoords = aTexCoords;
    gl_Position = model * vec4(aPos, 0.0, 1.0);
} 

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D shadowMap;

void main()
{ 
    float depthValue = texture(shadowMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}