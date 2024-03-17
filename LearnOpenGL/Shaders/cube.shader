#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

//out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};
uniform vec3 lightPos;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
}

#shader fragment
#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

//uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform float mixAmount = 0.25f;
//vec2 TexCoord2 = TexCoord;

uniform vec3 lightColor;
uniform vec3 objectColor;

float ambientStrength = 0.1;
float specularStrength = 0.7;
float diffStrength = 0.5;

void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffStrength * diff * lightColor;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;  
            
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0); // * mix(texture(texture0, TexCoord), texture(texture1, TexCoord2), mixAmount);
}