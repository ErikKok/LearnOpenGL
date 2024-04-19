#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140, binding = 0) uniform projectionUbo
{
    mat4 projection;   
};

uniform mat4 viewTranslationRemoved;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * viewTranslationRemoved * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}