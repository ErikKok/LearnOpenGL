#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewProjectionMatrixTranslationRemoved;

void main()
{
    TexCoords = aPos;
    vec4 pos = viewProjectionMatrixTranslationRemoved * vec4(aPos, 1.0f);
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