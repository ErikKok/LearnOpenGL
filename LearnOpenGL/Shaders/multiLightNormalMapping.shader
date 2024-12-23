#shader vertex
#version 450 core
#pragma debug(on)

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
//layout (location = 4) in vec3 aBitangent; // TODO not needed, remove from model loading and mesh

out VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosTangent;
    vec3 dirLightShadowCoord;               // clip space   // Orthographic
    vec3 dirLightDirectionTangent;
    vec3 pointLightPositionTangent[4];
    vec4 spotLightShadowCoord[2];           // clip space   // Perspective
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out;

layout(binding = 0, std430) readonly buffer dirLightMVPSSBO {
    mat4 dirLightMVP[]; };

layout(binding = 1, std430) readonly buffer spotLight0MVPSSBO {
    mat4 spotLight0MVP[]; };

layout(binding = 2, std430) readonly buffer spotLight1MVPSSBO {
    mat4 spotLight1MVP[]; };

layout(binding = 3, std430) readonly buffer normalMatrixSSBO {
    mat4 normalMatrix[]; };

layout(binding = 4, std430) readonly buffer ModelViewMatrixSSBO {
    mat4 modelViewMatrix[]; };

layout(binding = 5, std430) readonly buffer MVPMatrixSSBO {
    mat4 MVP[]; };

uniform vec3 dirLightDirection;      // View Space // normalized
uniform vec3 pointLightPosition[4];  // View Space // NOT normalized
uniform vec3 spotLightPosition[2];   // View Space // NOT normalized
uniform vec3 spotLightDirection[2];  // View Space // NOT normalized

void main()
{
    //mat4 ModelViewMatrix = view * model;
    //mat3 MV3x3 = mat3(modelViewMatrix[gl_InstanceID]);    
    vec3 T = mat3(normalMatrix[gl_InstanceID]) * normalize(aTangent);
    //vec3 B = mat3(modelViewMatrix[gl_InstanceID]) * normalize(aBitangent); // Better to calculate B?
    vec3 N = mat3(normalMatrix[gl_InstanceID]) * normalize(aNormal);
    T = normalize(T - N * dot(N, T)); // re-orthogonalize T with respect to N (Gram-Schmidt)
    vec3 B = cross(N, T); // then retrieve perpendicular vector B with the cross product of T and N
    mat3 TBN = transpose(mat3(T, B, N)); // This matrix goes from view space to tangent space
    
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosTangent = TBN * vec3(modelViewMatrix[gl_InstanceID] * vec4(aPos, 1.0f));
    // dirLight
    vs_out.dirLightShadowCoord = vec3(dirLightMVP[gl_InstanceID] * vec4(aPos, 1.0f));
    vs_out.dirLightDirectionTangent = TBN * dirLightDirection;
    // pointLight
    for (int i = 0; i < vs_out.pointLightPositionTangent.length(); i++)
        vs_out.pointLightPositionTangent[i] = TBN * pointLightPosition[i];
    // spotLight
    vs_out.spotLightShadowCoord[0] = spotLight0MVP[gl_InstanceID] * vec4(aPos, 1.0f);
    vs_out.spotLightShadowCoord[1] = spotLight1MVP[gl_InstanceID] * vec4(aPos, 1.0f);
    for (int i = 0; i < vs_out.spotLightPositionTangent.length(); i++) {
        vs_out.spotLightPositionTangent[i] = TBN * spotLightPosition[i];
        vs_out.spotLightDirectionTangent[i] = TBN * spotLightDirection[i];
    }

    gl_Position = MVP[gl_InstanceID] * vec4(aPos, 1.0f); // clip space
}

#shader fragment
#version 420 core
#pragma debug(on)

out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPosTangent;      
    vec3 dirLightShadowCoord;
    vec3 dirLightDirectionTangent;
    vec3 pointLightPositionTangent[4];
    vec4 spotLightShadowCoord[2];
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    //sampler2D height1;
    float shininess;                // Impacts the scattering/radius of the specular highlight
    sampler2D emissionTexture;      // Texture used used for emission on the object itself (no emissionMap involved, just 100% coverage)
    float emissionStrength;         // Strength for emission
    sampler2D lightEmissionMap;     // EmissionMap used for light emission
    sampler2D lightEmissionTexture; // Texture used for light emission
};   
uniform Material material;

struct DirLight {
    bool on;
    vec3 color;                     // Light color
    float strength;                 // Overall strength
    sampler2DShadow depthMap;
    float ambient;                  // Ambient strength
};
uniform DirLight dirLight;

struct PointLight {
    bool on;
    vec3 color;                     // Light color
    float strength;                 // Overall strength
    //sampler2D depthMap;
    float constant;                 // Usually kept at 1.0f
    float linear;                   // Short distance intensity
    float quadratic;                // Long distance intensity
};
uniform PointLight pointLights[4];  // TODO Hard limit pointlights count (max 166?)

struct SpotLight {
    bool on;
    vec3 color;                     // Light color
    float strength;                 // Overall strength
    sampler2DShadow depthMap;
    float constant;                 // Usually kept at 1.0f
    float linear;                   // Short distance intensity
    float quadratic;                // Long distance intensity
    float outerCutOff;              // Outer cone
    float emissionStrength;         // Overall strength
    float epsilon;                  // Gradually fade the light between inner and outer cone
};
uniform SpotLight spotLights[2];

vec3 normalTangentNormalized = normalize(texture(material.normal1, vs_out.TexCoords).rgb * 2.0 - 1.0);
vec3 FragPosTangentNormalized = normalize(vs_out.FragPosTangent);

vec3 textureDiffuse = vec3(texture(material.diffuse1, vs_out.TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, vs_out.TexCoords));

float cutOffSpecular = 0.01f;
float cutOffAttenuation = 0.01f;

vec3 CalcDirLight(DirLight light)
{
    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    float diff = max(dot(normalTangentNormalized, vs_out.dirLightDirectionTangent), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // Outside depthMap/frustum?
    if (vs_out.dirLightShadowCoord.x > 1.0f || vs_out.dirLightShadowCoord.x < -1.0f 
     || vs_out.dirLightShadowCoord.y > 1.0f || vs_out.dirLightShadowCoord.y < -1.0f 
     || vs_out.dirLightShadowCoord.z > 1.0f || vs_out.dirLightShadowCoord.z < -1.0f)
        return (ambient + diffuse) * light.strength;
    vec3 ShadowCoord = vs_out.dirLightShadowCoord.xyz * 0.5f + 0.5f;

    float shadow = texture(light.depthMap, ShadowCoord.xyz);

    // no more specular (calculations)
    if (ShadowCoord.z > 0.75f)
        return shadow * (ambient + diffuse) * light.strength;

    // specular
    vec3 halfwayDir = normalize(vs_out.dirLightDirectionTangent - FragPosTangentNormalized); // Blinn-Phong

    float spec = pow(max(dot(normalTangentNormalized, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    return shadow * (ambient + diffuse + specular) * light.strength;
}

vec3 CalcPointLight(PointLight light, int i)
{
    // attenuation
    float distance = length(vs_out.pointLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation = (attenuation - cutOffAttenuation) / (1.0f - cutOffAttenuation);
    attenuation = max(attenuation, 0.0f);

    vec3 lightDir = normalize(vs_out.pointLightPositionTangent[i] - vs_out.FragPosTangent);

    // diffuse
    float diff = max(dot(normalTangentNormalized, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;
    diffuse *= attenuation;

    // specular
    vec3 specular;
    if (attenuation > cutOffSpecular) {
        vec3 halfwayDir = normalize(lightDir - FragPosTangentNormalized); // Blinn-Phong
        float spec = pow(max(dot(normalTangentNormalized, halfwayDir), 0.0f), material.shininess);
        specular = light.color * spec * textureSpecular;
        specular *= attenuation;
    }

    return (diffuse + specular) * light.strength;
}

vec3 CalcSpotLight(SpotLight light, int i)
{   
    vec3 ShadowCoord = vs_out.spotLightShadowCoord[i].xyz / vs_out.spotLightShadowCoord[i].w;
    // Outside depthMap/frustum?
    if (ShadowCoord.x > 1.0f || ShadowCoord.x < -1.0f || ShadowCoord.y > 1.0f  || ShadowCoord.y < -1.0f || ShadowCoord.z > 1.0f || ShadowCoord.z < 0.0f)
        return vec3(0.0f);
    ShadowCoord = ShadowCoord * 0.5f + 0.5f;

    float shadow = texture(light.depthMap, ShadowCoord.xyz);

    // attenuation
    float distance = length(vs_out.spotLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));     
    attenuation = (attenuation - cutOffAttenuation) / (1.0f - cutOffAttenuation);
    attenuation = max(attenuation, 0.0f);

    vec3 lightDir = normalize(vs_out.spotLightPositionTangent[i] - vs_out.FragPosTangent);

    // cone
    float theta = dot(lightDir, normalize(-vs_out.spotLightDirectionTangent[i]));
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);

    // diffuse
    float diff = max(dot(normalTangentNormalized, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;
    diffuse *= attenuation;
    diffuse *= intensity;

    // specular
    vec3 specular;
    if (attenuation > cutOffSpecular) {
        vec3 halfwayDir = normalize(lightDir - FragPosTangentNormalized); // Blinn-Phong
        float spec = pow(max(dot(normalTangentNormalized, halfwayDir), 0.0f), material.shininess);
        vec3 specular = light.color * spec * textureSpecular;
        specular *= intensity;
        specular *= attenuation;
    }

    // emission
    vec3 emission;
    if (light.emissionStrength > 0.0f) {
        vec3 textureLightEmissionMap = vec3(texture(material.lightEmissionMap, vs_out.TexCoords));
        vec3 textureLightEmissionTexture = vec3(texture(material.lightEmissionTexture, vs_out.TexCoords));
        emission = textureLightEmissionMap.r * light.emissionStrength * textureLightEmissionTexture;
        emission *= shadow;
        emission *= intensity;
        emission *= attenuation;
    }

    return shadow * (diffuse + specular + emission) * light.strength;
}

void main()
{
    vec3 resultDirLight;
    if (dirLight.on)
        resultDirLight = CalcDirLight(dirLight);

    vec3 resultPointLight;
    for(int i = 0; i < pointLights.length(); i++) {
        if (pointLights[i].on)
            resultPointLight += CalcPointLight(pointLights[i], i);
    }

    vec3 resultSpotLight;
    for(int i = 0; i < spotLights.length(); i++) {
        if (spotLights[i].on)
            resultSpotLight += CalcSpotLight(spotLights[i], i);
    }   

    // emission - no emissionMap used, just 100% coverage
    vec3 resultEmissionMaterial = texture(material.emissionTexture, vs_out.TexCoords).rgb * material.emissionStrength;

    FragColor = vec4(resultDirLight + resultSpotLight + resultPointLight + resultEmissionMaterial, 1.0f);
}