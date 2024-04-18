#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

layout (std140, binding = 0) uniform projectionView // TODO uniform met maar 1 member?
{                                                   // verschillende types accepteren
    mat4 projection;                                // zie BufferSubData.h
    //mat4 modelView;
    //mat3 NormalViewCPU;
    //mat4 View;
};

layout(binding = 2, std430) readonly buffer ssboModelView {
    mat4 modelView[];
};

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * modelView[gl_InstanceID] * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
    //FragColor = vec4(1.0, 0.28, 0.26, 1.0);
}