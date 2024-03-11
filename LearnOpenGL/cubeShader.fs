#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixAmount = 0.25f;
vec2 TexCoord2 = TexCoord;

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord2), mixAmount);
}