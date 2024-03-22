#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 FragPosView;
out vec3 FragPosWorld;
out vec3 NormalView;
out vec3 NormalWorld;
//out vec3 LightPosView;
//out vec3 LightPosWorld;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

//uniform vec3 lightPos = vec3(0.0f, 0.0f, 0.0f); // world space
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    FragPosView = vec3(view * model * vec4(aPos, 1.0));
    FragPosWorld = vec3(model * vec4(aPos, 1.0));
    NormalView = mat3(transpose(inverse(view * model))) * aNormal;
    NormalWorld = mat3(transpose(inverse(model))) * aNormal;
    //LightPosView = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    //LightPosWorld = lightPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosView; // positie fragment (Vertex) in view space
in vec3 FragPosWorld;
in vec3 NormalView;
in vec3 NormalWorld;
//in vec3 LightPosView;
//in vec3 LightPosWorld; // positie lightcube in view space, maar niet normalized!?

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};   
uniform Material material;

struct DirLight {
    vec3 direction;
	
    //vec3 ambient;
    vec3 diffuse;
    //vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light); 

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    //vec3 ambient;
    vec3 diffuse;
    //vec3 specular;
};
//#define NR_POINT_LIGHTS 4
const int pointLightsCount = 4;
uniform PointLight pointLights[pointLightsCount];
vec3 CalcPointLight(PointLight light);

struct SpotLight {
    vec3 position;      // World space
    vec3 direction;     // World space
    float cutOff;
    float outerCutOff;

    //vec3 ambient;
    vec3 diffuse;
    //vec3 specularStrength;

    // attenuation values
    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light);

struct FlashLight {
    //vec3 position; // camera.getPosition() // in view space positon(0,0,0)
    //vec3 direction; // camera.getFront() // in view space positon(0,0,-1)
    float cutOff;
    float outerCutOff;

    //vec3 ambient;
    vec3 diffuse;
    //vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform FlashLight flashLight;
vec3 CalcFlashLight(FlashLight light);

vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 norm = normalize(NormalWorld);
vec3 viewDir = normalize(cameraPosition - FragPosWorld); // from what direction the player is looking at the fragment

vec3 textureDiffuse = vec3(texture(material.diffuse, TexCoords));
vec3 textureSpecular = vec3(texture(material.specular, TexCoords));

void main()
{
    vec3 resultDirectionalLight = CalcDirLight(dirLight);
    vec3 resultPointLight;
    for(int i = 0; i < pointLightsCount; i++)
        resultPointLight += CalcPointLight(pointLights[i]);    
    vec3 resultSpotLight = CalcSpotLight(spotLight);
    vec3 resultFlashLight = CalcFlashLight(flashLight);

    vec3 ambient = vec3(0.0f);
    FragColor = vec4(ambient + resultDirectionalLight + resultSpotLight + resultPointLight + resultFlashLight, 1.0);
}

vec3 CalcDirLight(DirLight light)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    vec3 specular = light.diffuse * spec * textureSpecular;
    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position - FragPosWorld);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - FragPosWorld);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    vec3 specular = light.diffuse * spec * textureSpecular;
    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light)
{   
    vec3 lightDir = normalize(light.position - FragPosWorld);
    // ambient
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;

    // emission
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0) {
    emission = vec3(texture(material.emission, TexCoords)) * 0.25f;
    }

    // spot Light
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;

    float distance    = length(light.position - FragPosWorld);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    
    //ambient  *= attenuation; // if we were to use more than 1 light source all the ambient components will start to stack up
    diffuse  *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (diffuse + specular + emission);
}

vec3 CalcFlashLight(FlashLight light)
{   
    vec3 lightDir = normalize(cameraPosition - FragPosView);

    // diffuse 
    vec3 normView = normalize(NormalView);
    float diff = max(dot(normView, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;

    // emission
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0) {
    emission = vec3(texture(material.emission, TexCoords)) * 0.25f; // emission(strenght)
    }

    // flash Light
    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f);
    float theta = dot(lightDir, normalize(cameraDirection));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;

    float distance    = length(cameraPosition - FragPosView);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    
    //ambient  *= attenuation; // if we were to use more than 1 light source all the ambient components will start to stack up
    diffuse  *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (diffuse + specular + emission);
}