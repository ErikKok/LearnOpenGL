#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPosView;
//out vec3 FragPosWorld;
out vec3 NormalView;
//out vec3 NormalWorld;
out mat4 View;
//out vec3 LightPosView;
//out vec3 LightPosWorld;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

//uniform vec3 lightPos = vec3(0.0f, 0.0f, 0.0f); // world space
uniform mat4 model;

uniform mat3 NormalViewCPU;
//uniform mat3 NormalWorldCPU;

void main()
{
    TexCoords = aTexCoords;    
    FragPosView = vec3(view * model * vec4(aPos, 1.0));
    //FragPosWorld = vec3(model * vec4(aPos, 1.0));
    //NormalView = mat3(transpose(inverse(view * model))) * aNormal;
    //NormalWorld = mat3(transpose(inverse(model))) * aNormal;
    NormalView = NormalViewCPU * aNormal;
    //NormalWorld = NormalWorldCPU * aNormal;
    View = view;

    //LightPosView = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    //LightPosWorld = lightPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosView; // positie fragment (Vertex) in view space
//in vec3 FragPosWorld;
in vec3 NormalView;
//in vec3 NormalWorld;
in mat4 View; // TODO naar CPU
//in vec3 LightPosView;
//in vec3 LightPosWorld; // positie lightcube in view space, maar niet normalized!?

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1; // was emission
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
uniform PointLight pointLights[64];
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
//vec3 normalWorld = normalize(NormalWorld);
vec3 normalView = normalize(NormalView);
//An advantage of view space is that the viewer's position is always at (0,0,0) so you already got the position of the viewer for free.
// is viewDirView tlekens nul???
//vec3 viewDirView = normalize(cameraPosition - FragPosView); // from what direction the player is looking at the fragment
vec3 viewDirView = normalize(-FragPosView);

vec3 textureDiffuse = vec3(texture(material.diffuse1, TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, TexCoords));
vec3 textureEmission = vec3(texture(material.normal1, TexCoords));

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

//vec3 CalcDirLight(DirLight light) // World space
//{
//    vec3 lightDir = normalize(light.direction);
//    // diffuse shading
//    float diff = max(dot(normalWorld, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normalWorld);
//    vec3 viewDirWorld = normalize(cameraPosition - FragPosWorld);
//    float spec = pow(max(dot(viewDirWorld, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//
//    return (diffuse + specular);
//}

vec3 CalcDirLight(DirLight light) // View space II
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normalView);
    //vec3 viewDirWorld = normalize(cameraPosition - FragPosView);
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;

    return (diffuse + specular);
}

//vec3 CalcDirLight(DirLight light) // View space // https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/directional-lights-i/
//{
//    vec3 viewDir2 = normalize(FragPosView);
//    
//    vec3 lightDir = normalize(light.direction);
//    // diffuse shading
//    float diff = max(dot(NormalView, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, NormalView);
//    float spec = pow(max(dot(viewDir2, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//
//    return (diffuse + specular);
//}


//vec3 CalcPointLight(PointLight light) // World space
//{
//    vec3 lightDir = normalize(light.position - FragPosWorld);
//    // diffuse shading
//    float diff = max(dot(normalWorld, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normalWorld);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//    // attenuation
//    float distance = length(light.position - FragPosWorld);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // combine results
//
//    diffuse *= attenuation;
//    specular *= attenuation;
//
//    return (diffuse + specular);
//}

vec3 CalcPointLight(PointLight light) // View space
{
    vec3 LightPosView = vec3(View * vec4(light.position, 1.0));
    vec3 lightDir = normalize(LightPosView - FragPosView);
    // diffuse shading
    float diff = max(dot(NormalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, NormalView); // first vector to point from the light source towards the fragment's position
    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // attenuation
    float distance = length(LightPosView - FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

//vec3 CalcSpotLight(SpotLight light) // World space
//{   
//    vec3 lightDir = normalize(light.position - FragPosWorld);
//    // diffuse
//    float diff = max(dot(normalWorld, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular
//    vec3 reflectDir = reflect(-lightDir, normalWorld);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//    // emission
//    vec3 emission = textureEmission * light.emission;
//    // cone
//    float theta = dot(lightDir, normalize(-light.direction));
//    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0f, 1.0f);
//    diffuse *= intensity;
//    specular *= intensity;
//    emission *= intensity;
//    // attenuation
//    float distance = length(light.position - FragPosWorld);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
//    diffuse  *= attenuation;
//    specular *= attenuation;
//    emission *= attenuation;
//
//    return (diffuse + specular + emission);
//}

vec3 CalcSpotLight(SpotLight light) // View space
{   
    //vec3 LightPosView = vec3(View * vec4(light.position, 1.0));

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

vec3 CalcFlashLight(FlashLight light) // View space
{   
    vec3 lightDir = normalize(cameraPosition - FragPosView);
    // diffuse 
    //vec3 normalView = normalize(NormalView); // normalize NormalView lijkt niet nodig?
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