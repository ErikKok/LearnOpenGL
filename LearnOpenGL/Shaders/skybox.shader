#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140, binding = 0) uniform projectionView // TODO uniform met maar 1 member?
{                                                   // verschillende types accepteren
    mat4 projection;                                // zie BufferSubData.h
    //mat4 modelView;
    //mat3 NormalViewCPU;
    //mat4 View;
};

uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
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