#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 FragPosView;
out vec3 NormalView;

layout (std140, binding = 0) uniform projectionView // TODO uniform met maar 1 member?
{                                                   // verschillende types accepteren
    mat4 projection;                                // zie BufferSubData.h
    //mat4 modelView;
    //mat3 NormalViewCPU;
    //mat4 View;
};

//uniform mat4 model;
uniform mat4 modelView;
uniform mat3 NormalViewCPU; // rename NormalMatrix?

void main()
{
    TexCoords = aTexCoords; // TODO kan dit niet direct naar de fragment shader ipv via de vertex shader?
    FragPosView = vec3(modelView * vec4(aPos, 1.0)); // TODO view * model kan op cpu, gebeurd toch al voor NormalViewCPU
    NormalView = NormalViewCPU * aNormal;
    gl_Position = projection * modelView * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosView;
in vec3 NormalView;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    //sampler2D normal1;
    //sampler2D height1;
    sampler2D emission;
    float shininess;
};   
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 diffuse;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light); 

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 diffuse;
};
//#define NR_POINT_LIGHTS 4
uniform int pointLightsCount;
uniform PointLight pointLights[64]; // Harde limiet aantal pointlights
vec3 CalcPointLight(PointLight light);

struct SpotLight {
    vec3 position;      
    vec3 direction;
    float outerCutOff;
    float epsilon;

    vec3 diffuse;
    float emission;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light);

struct FlashLight {
    bool on;
    float outerCutOff;
    float epsilon;

    vec3 diffuse;
    float emission;

    float constant;
    float linear;
    float quadratic;
};
uniform FlashLight flashLight;
vec3 CalcFlashLight(FlashLight light);

vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 normalView = normalize(NormalView);
vec3 viewDirView = normalize(-FragPosView);

vec3 textureDiffuse = vec3(texture(material.diffuse1, TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, TexCoords));
vec3 textureEmission = vec3(texture(material.emission, TexCoords));

void main()
{
    vec3 resultDirectionalLight = CalcDirLight(dirLight) * 2.0f; // TODO
    vec3 resultPointLight;
    for(int i = 0; i < pointLightsCount; i++)
        resultPointLight += CalcPointLight(pointLights[i]) * 3.0f; // TODO 
    vec3 resultSpotLight = CalcSpotLight(spotLight) * 1.0f; // TODO
    vec3 resultFlashLight = vec3(0.0f);
    if (flashLight.on)
        resultFlashLight = CalcFlashLight(flashLight);       

    vec3 ambient = vec3(0.0f); // TODO
    FragColor = vec4(ambient + resultDirectionalLight + resultSpotLight + resultPointLight + resultFlashLight, 1.0);
}

vec3 CalcDirLight(DirLight light)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normalView);
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;

    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position - FragPosView);
    // diffuse shading
    float diff = max(dot(NormalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, NormalView); // first vector to point from the light source towards the fragment's position
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // attenuation
    float distance = length(light.position - FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light)
{   
    vec3 lightDir = normalize(light.position - FragPosView);
    // diffuse
    float diff = max(dot(NormalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 reflectDir = reflect(-lightDir, NormalView);
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // emission
    vec3 emission = textureEmission * light.emission;
    // cone
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;
    // attenuation
    float distance = length(light.position - FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
    diffuse  *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (diffuse + specular + emission);
}

vec3 CalcFlashLight(FlashLight light)
{   
    vec3 lightDir = normalize(cameraPosition - FragPosView);
    // diffuse 
    float diff = max(dot(NormalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 reflectDir = reflect(-lightDir, NormalView);
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // emission
    vec3 emission = vec3(0.0f);
    if (textureSpecular.r == 0.0f) { // niet de metalen rand vd container
        emission = textureEmission * light.emission; 
    }
    // cone
    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
    float theta = dot(lightDir, cameraDirection); // normalize cameraDirection lijkt niet nodig?
    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;
    // attenuation
    float distance = length(cameraPosition - FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    diffuse  *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (diffuse + specular + emission);
}