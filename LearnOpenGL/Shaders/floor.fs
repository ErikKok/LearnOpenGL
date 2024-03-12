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