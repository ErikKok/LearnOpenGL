#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 Color;
out vec2 TexCoord;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Color = aColor;
    TexCoord = aTexture;
}
#shader fragment
#version 330 core
in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture2;

void main()
{
    //FragColor = vec4(Color, 1.0f);
    //FragColor = texture(texture2, TexCoord);
    FragColor = texture(texture2, TexCoord) * vec4(Color, 0.15); 
}