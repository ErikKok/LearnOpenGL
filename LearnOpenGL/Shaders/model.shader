#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//out mat4 View;


layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

//uniform vec3 lightPos = vec3(0.0f, 0.0f, 0.0f); // world space
uniform mat4 model;

//uniform mat3 NormalViewCPU;
//uniform mat3 NormalWorldCPU;

void main()
{
    TexCoords = aTexCoords;    
    //FragPosView = vec3(view * model * vec4(aPos, 1.0));
    //FragPosWorld = vec3(model * vec4(aPos, 1.0));
    //NormalView = mat3(transpose(inverse(view * model))) * aNormal;
    //NormalWorld = mat3(transpose(inverse(model))) * aNormal;
    //NormalView = NormalViewCPU * aNormal;
    //NormalWorld = NormalWorldCPU * aNormal;
    //View = view;

    //LightPosView = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    //LightPosWorld = lightPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
//in vec3 FragPosView; // positie fragment (Vertex) in view space
//in vec3 FragPosWorld;
//in vec3 NormalView;
//in vec3 NormalWorld;
//in mat4 View; // TODO naar CPU
//in vec3 LightPosView;
//in vec3 LightPosWorld; // positie lightcube in view space, maar niet normalized!?

struct Material {
    sampler2D diffuse1;
    //sampler2D specular1;
    //sampler2D normal1; // was emission
    //float shininess;
};   
uniform Material material;


void main()
{
    FragColor = texture(material.diffuse1, TexCoords);
}

