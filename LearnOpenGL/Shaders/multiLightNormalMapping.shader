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
    vec4 dirLightShadowCoord;           // clip space   // Orthographic
    vec3 dirLightDirectionTangent;
    vec4 spotLight0ShadowCoord;         // clip space   // Perspective
    vec4 spotLight1ShadowCoord;         // clip space   // Perspective
    vec3 pointLightPositionTangent[4];
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out;

//layout(binding = 2, std430) readonly buffer NormalMatrixSSBO {
//    mat4 normalMatrix[];
//};
//
//layout(binding = 3, std430) readonly buffer ModelViewMatrixSSBO {
//    mat4 modelViewMatrix[];
//};
//
//layout(binding = 4, std430) readonly buffer MVPMatrixSSBO {
//    mat4 MVPMatrix[];
//};
//
//layout(binding = 5, std430) readonly buffer dirLightMVPMatrixSSBO {
//    mat4 dirLightMVPMatrix[];
//};
//
//layout(binding = 6, std430) readonly buffer spotLightMVPMatrixSSBO {
//    mat4 spotLightMVPMatrix[];
//};
//
//layout(binding = 7, std430) readonly buffer flashLightMVPMatrixSSBO {
//    mat4 flashLightMVPMatrix[];
//};

layout(binding = 24, std430) readonly buffer uberSSBO {
    mat4 dirLightMVPMatrix[10];
    mat4 flashLightMVPMatrix[10];
    mat4 spotLightMVPMatrix[10];
    mat4 normalMatrix[10];
    mat4 modelViewMatrix[10];
    mat4 MVPMatrix[10];
};

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
    vs_out.dirLightShadowCoord = dirLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    vs_out.dirLightDirectionTangent = TBN * dirLightDirection;
    // pointLight
    for (int i = 0; i < vs_out.pointLightPositionTangent.length(); i++)
        vs_out.pointLightPositionTangent[i] = TBN * pointLightPosition[i];
    // spotLight
    vs_out.spotLight0ShadowCoord = flashLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    vs_out.spotLight1ShadowCoord = spotLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    for (int i = 0; i < vs_out.spotLightPositionTangent.length(); i++) {
        vs_out.spotLightPositionTangent[i] = TBN * spotLightPosition[i];
        vs_out.spotLightDirectionTangent[i] = TBN * spotLightDirection[i];
    }

    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f); // clip space
}

#shader fragment
#version 420 core
#pragma debug(on)

out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPosTangent;      
    vec4 dirLightShadowCoord;
    vec3 dirLightDirectionTangent;
    vec4 spotLight0ShadowCoord;
    vec4 spotLight1ShadowCoord;
    vec3 pointLightPositionTangent[4];
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    //sampler2D height1;
    float shininess;                // Impacts the scattering/radius of the specular highlight
    sampler2D emission;             // Texture used used for emission on the object itself (no emissionMap involved, just 100% coverage)
    float emissionStrength;         // Strength for emission
    sampler2D lightEmissionMap;     // EmissionMap used for spotLight emission
    sampler2D lightEmissionTexture; // Texture used for spotLight emission
};   
uniform Material material;

struct DirLight {
    bool on;
    vec3 color;                     // Light color
    float strength;                 // Overall strength
    sampler2D depthMap;
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
    sampler2D depthMap;
    float constant;                 // Usually kept at 1.0f
    float linear;                   // Short distance intensity
    float quadratic;                // Long distance intensity
    float outerCutOff;              // Outer cone
    float emissionStrength;         // Overall strength
    float epsilon;                  // Gradually fade the light between inner and outer cone
};
uniform SpotLight spotLights[2];

vec3 normalTangent = normalize(texture(material.normal1, vs_out.TexCoords).rgb * 2.0 - 1.0);

vec3 textureDiffuse = vec3(texture(material.diffuse1, vs_out.TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, vs_out.TexCoords));

vec3 CalcDirLight(DirLight light)
{
    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    float diff = max(dot(normalTangent, vs_out.dirLightDirectionTangent), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(vs_out.dirLightDirectionTangent - vs_out.FragPosTangent); // Blinn-Phong

    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // shadow - 1.0f = no shadow, 0.0f = full shadow
    float shadow = 0.0f;
    vec4 ShadowCoord = vs_out.dirLightShadowCoord * 0.5f + 0.5f;
    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
        }    
    }
    shadow /= 9.0f;
    if(ShadowCoord.z > 1.0f)
        shadow = 1.0f;

    return shadow * (ambient + diffuse + specular) * light.strength;
}

vec3 CalcPointLight(PointLight light, int i)
{
    vec3 lightDir = normalize(vs_out.pointLightPositionTangent[i] - vs_out.FragPosTangent);
    
    // diffuse
    float diff = max(dot(normalTangent, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir - vs_out.FragPosTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // attenuation
    float distance = length(vs_out.pointLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.strength;
}

vec3 CalcSpotLight(SpotLight light, int i)
{   
    vec3 lightDir = normalize(vs_out.spotLightPositionTangent[i] - vs_out.FragPosTangent);

    // diffuse
    float diff = max(dot(normalTangent, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir - vs_out.FragPosTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // cone
    float theta = dot(lightDir, normalize(-vs_out.spotLightDirectionTangent[i]));
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(vs_out.spotLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    diffuse  *= attenuation;
    specular *= attenuation;

    // shadow - 1.0f = no shadow, 0.0f = full shadow
    float shadow = 0.0f;
    vec3 ShadowCoord;
    if (i == 0)
        ShadowCoord = vs_out.spotLight0ShadowCoord.xyz / vs_out.spotLight0ShadowCoord.w;
    else
       ShadowCoord = vs_out.spotLight1ShadowCoord.xyz / vs_out.spotLight1ShadowCoord.w;
    ShadowCoord = ShadowCoord * 0.5f + 0.5f;
    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
        }    
    }
    shadow /= 9.0f;
    if(ShadowCoord.z > 1.0f)
        shadow = 1.0f;

    // emission
    vec3 emission;
    if(light.emissionStrength > 0.0f) {
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
    vec3 emissionMaterial = texture(material.emission, vs_out.TexCoords).rgb * material.emissionStrength;

    FragColor = vec4(resultDirLight + resultSpotLight + resultPointLight + emissionMaterial, 1.0);
}