#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
//out vec2 TexCoord;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(model))) * aNormal;  
    //TexCoords = aTexCoords;
    Normal = aNormal;
    gl_Position = projection * view vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
//uniform sampler2D texture2;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 