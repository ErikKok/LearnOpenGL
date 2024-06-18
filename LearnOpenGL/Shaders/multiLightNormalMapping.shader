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
    vec4 dirLightShadowCoord;       // clip space   // Orthographic
    vec3 dirLightDirectionTangent;
    vec4 spotLightShadowCoord;      // clip space   // Perspective
    //vec3 spotLightPositionTangent;
    vec4 flashLightShadowCoord;     // clip space   // Perspective
    //vec3 cameraDirectionTangent;
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

uniform vec3 dirLightDirection;     // View Space // normalized
uniform vec3 pointLightPosition[4]; // View Space // NOT normalized
uniform vec3 spotLightPosition[2];  // View Space // NOT normalized
uniform vec3 spotLightDirection[2];  // View Space // NOT normalized

out VS_OUT2 { // PASS_THROUGH_GS
    vec3 pointLightPositionTangent[4];
} vs_out2;

out VS_OUT3 { // PASS_THROUGH_GS
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out3;

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
    vs_out.dirLightShadowCoord = dirLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    vs_out.dirLightDirectionTangent = TBN * dirLightDirection;
    for (int i = 0; i < vs_out2.pointLightPositionTangent.length(); i++)
        vs_out2.pointLightPositionTangent[i] = TBN * pointLightPosition[i];
    vs_out.spotLightShadowCoord = spotLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    for (int i = 0; i < vs_out3.spotLightPositionTangent.length(); i++) {
        vs_out3.spotLightPositionTangent[i] = TBN * spotLightPosition[i];
        vs_out3.spotLightDirectionTangent[i] = TBN * spotLightDirection[i];
    }
    vs_out.flashLightShadowCoord = flashLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    //vs_out.cameraDirectionTangent = TBN * vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f); // clip space
}

#shader fragment
#version 420 core
#pragma debug(on)

out vec4 FragColor;

in VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosTangent;      
    vec4 dirLightShadowCoord;
    vec3 dirLightDirectionTangent;
    vec4 spotLightShadowCoord;
    //vec3 spotLightPositionTangent;
    vec4 flashLightShadowCoord;
    //vec3 cameraDirectionTangent;
} vs_out;

in VS_OUT2 { // PASS_THROUGH_GS
    vec3 pointLightPositionTangent[4];
} vs_out2;

in VS_OUT3 { // PASS_THROUGH_GS
    vec3 spotLightPositionTangent[2];
    vec3 spotLightDirectionTangent[2];
} vs_out3;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    //sampler2D height1;
    sampler2D emission;         // Texture used used for emission on the object itself (no emissionMap involved, just 100% coverage)
    float emissionStrength;     // Strength for emission
    float shininess;            // Impacts the scattering/radius of the specular highlight
    sampler2D flashLightEmissionMap;    // EmissionMap used for flashLight emission
    sampler2D flashLightEmissionTexture; // Texture used for flashLight emission
};   
uniform Material material;

struct DirLight {
    vec3 color;         // Light color
    float ambient;      // Ambient strength
    float strength;     // Overall strength
    sampler2D depthMap;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 color;         // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
};
uniform PointLight pointLights[4]; // TODO Hard limit pointlights count (max 166?)

struct SpotLight {
    bool on;
    //vec3 direction;     // View Space
    float outerCutOff;  // Outer cone
    float epsilon;      // Gradually fade the light between inner and outer cone
    vec3 color;         // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
    float emissionStrength;     // Overall strength
    sampler2D depthMap;
};
uniform SpotLight spotLights[2];

//struct FlashLight {
//    bool on;            // Flashlight on or off
//    vec3 direction;     // View Space
//    float outerCutOff;  // Outer cone
//    float epsilon;      // Gradually fade the light between inner and outer cone
//    vec3 color;         // Light color
//    float constant;     // Usually kept at 1.0f
//    float linear;       // Short distance intensity
//    float quadratic;    // Long distance intensity
//    float strength;     // Overall strength
//    //vec3 origin;        // Needs to be converted to Tangent space if not 0,0,0!
//    float emissionStrength;     // Overall strength
//    sampler2D depthMap;
//};
//uniform FlashLight flashLight;

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
    vec3 lightDir = normalize(vs_out2.pointLightPositionTangent[i] - vs_out.FragPosTangent);
    
    // diffuse
    float diff = max(dot(normalTangent, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir - vs_out.FragPosTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // attenuation
    float distance = length(vs_out2.pointLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.strength;
}

vec3 CalcSpotLight(SpotLight light, int i)
{   
    vec3 lightDir = normalize(vs_out3.spotLightPositionTangent[i] - vs_out.FragPosTangent);

    // diffuse
    float diff = max(dot(normalTangent, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir - vs_out.FragPosTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // cone
    float theta = dot(lightDir, normalize(-vs_out3.spotLightDirectionTangent[i]));
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(vs_out3.spotLightPositionTangent[i] - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    diffuse  *= attenuation;
    specular *= attenuation;

    // shadow - 1.0f = no shadow, 0.0f = full shadow
    float shadow = 0.0f;
    vec3 ShadowCoord = vs_out.spotLightShadowCoord.xyz / vs_out.spotLightShadowCoord.w;
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
    vec3 textureFlashlightEmissionMap = vec3(texture(material.flashLightEmissionMap, vs_out.TexCoords));
    vec3 textureFlashlightEmissionTexture = vec3(texture(material.flashLightEmissionTexture, vs_out.TexCoords));
    vec3 emission = textureFlashlightEmissionMap.r * light.emissionStrength * textureFlashlightEmissionTexture;
    emission *= shadow;
    emission *= intensity;
    emission *= attenuation;

    return shadow * (diffuse + specular + emission) * light.strength;
}

//vec3 CalcFlashLight(FlashLight light)
//{
//    vec3 lightDir = normalize(light.origin - vs_out.FragPosTangent);
//
//    // diffuse
//    float diff = max(dot(normalTangent, lightDir), 0.0f);
//    vec3 diffuse = light.color * diff * textureDiffuse;
//
//    // specular
//    vec3 halfwayDir = normalize(lightDir - vs_out.FragPosTangent); // Blinn-Phong
//    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
//    vec3 specular = light.color * spec * textureSpecular;
//
//    // cone
//    float theta = dot(lightDir, normalize(vs_out.cameraDirectionTangent));
//    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);
//    diffuse *= intensity;
//    specular *= intensity;
//
//    // attenuation
//    float distance = length(light.origin - vs_out.FragPosTangent); // TODO could be done in world space
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    diffuse *= attenuation;
//    specular *= attenuation;   
//
//    // shadow - 1.0f = no shadow, 0.0f = full shadow
//    float shadow = 0.0f;
//    vec3 ShadowCoord = vs_out.flashLightShadowCoord.xyz / vs_out.flashLightShadowCoord.w;
//    ShadowCoord = ShadowCoord * 0.5 + 0.5;
//    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
//    for(int x = -1; x <= 1; ++x) {
//        for(int y = -1; y <= 1; ++y) {
//            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
//        }    
//    }
//    shadow /= 9.0f;    
//    if(ShadowCoord.z > 1.0f) {
//        shadow = 1.0f;
//    }
//
//    // emission
//    vec3 textureFlashlightEmissionMap = vec3(texture(material.flashLightEmissionMap, vs_out.TexCoords));
//    vec3 textureFlashlightEmissionTexture = vec3(texture(material.flashLightEmissionTexture, vs_out.TexCoords));
//    vec3 emission = textureFlashlightEmissionMap.r * light.emissionStrength * textureFlashlightEmissionTexture;
//    emission *= shadow;
//    emission *= intensity;
//    emission *= attenuation;
//
//    return shadow * (diffuse + specular + emission) * light.strength;
//}

void main()
{
    vec3 resultDirLight = CalcDirLight(dirLight);

    vec3 resultPointLight;
    for(int i = 0; i < pointLights.length(); i++)
        resultPointLight += CalcPointLight(pointLights[i], i);

    //vec3 resultSpotLight = CalcSpotLight(spotLight);
    vec3 resultSpotLight;
    for(int i = 0; i < spotLights.length(); i++) {
        if (spotLights[i].on)
            resultSpotLight += CalcSpotLight(spotLights[i], i);
    }

//    vec3 resultFlashLight = vec3(0.0f);
//    if (flashLight.on)
//        resultFlashLight = CalcFlashLight(flashLight);       

    // emission - no emissionMap used, just 100% coverage
    vec3 emissionMaterial = texture(material.emission, vs_out.TexCoords).rgb * material.emissionStrength;

    FragColor = vec4(resultDirLight + resultSpotLight + resultPointLight + emissionMaterial, 1.0);//+ resultFlashLight 
}