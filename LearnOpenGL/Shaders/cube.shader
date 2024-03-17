#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal; // <------- enige verschil met cubeShader (1 ipv 2) + geen textures

out vec2 TexCoords;
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
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    TexCoords = aTexCoords;
    LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    float shininess;
};   
uniform Material material;

struct Light {
   // vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;  

void main()
{
    // ambient
    //vec3 ambient = light.ambient * material.ambient;
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    //vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb; 
            
    //vec3 result = ambient + diffuse + specular;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0); // * mix(texture(texture0, TexCoord), texture(texture1, TexCoord2), mixAmount);
}