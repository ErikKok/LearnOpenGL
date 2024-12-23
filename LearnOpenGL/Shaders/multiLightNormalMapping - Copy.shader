#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosTangent;               // view space
    //vec3 NormalView;                // view space
    vec4 dirLightShadowCoord;       // clip space   // Orthographic
    vec4 spotLightShadowCoord;      // clip space   // Perspective
    //vec4 flashLightShadowCoord;     // clip space   // Perspective
    mat3 TBN;
    //mat4 view;
    vec3 lightDirTangent;
    vec3 lightPositionTangent;
    vec3 viewDirTangent;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
//uniform vec3 lightDir;

void main()
{
    mat4 ModelViewMatrix = view * model;
    mat3 MV3x3 = mat3(ModelViewMatrix);
    
    vec3 vertexNormal_cameraspace = MV3x3 * normalize(aNormal);
    vec3 vertexTangent_cameraspace = MV3x3 * normalize(aTangent);
    vec3 vertexBitangent_cameraspace = MV3x3 * normalize(aBitangent);

    mat3 TBN = transpose(mat3(
    vertexTangent_cameraspace,
    vertexBitangent_cameraspace,
    vertexNormal_cameraspace
    ));
    vs_out.TBN = TBN;

    vec3 lightPositionTangent = TBN * lightPos;
    vec3 lightDirTangent = TBN * vec3(0.0f, -1.0f, 0.0f);
    //vec3 viewDirTangent = TBN * vec3(view * model * vec4(aPos, 1.0f)); // FragPosTangent
    vs_out.FragPosTangent = TBN * vec3(view * model * vec4(aPos, 1.0f));



    //vec3 FragPos = vec3(model * vec4(aPos, 1.0));   
    //vs_out.TangentFragPos = TBN * FragPos;

    //vs_out.view = view;
    vs_out.TexCoords = aTexCoords;
    //vs_out.NormalView = mat3(NormalMatrix[gl_InstanceID]) * aNormal;
    //vs_out.FragPosWorld = vec3(model * vec4(aPos, 1.0f)); // als ik TBN *  doe dan beweegt de flashlight niet met de muis...
    //vs_out.FragPosView = vec3(ModelViewMatrix[gl_InstanceID] * vec4(aPos, 1.0f)); // als ik TBN *  doe dan beweegt de flashlight niet met de muis...
    //vs_out.dirLightShadowCoord = dirLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    //vs_out.spotLightShadowCoord = spotLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    //vs_out.flashLightShadowCoord = flashLightMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f);
    //gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0f); // clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0); // clip space
}

#shader fragment
#version 420 core
out vec4 FragColor;

in VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosTangent;
    //vec3 NormalView;
    vec4 dirLightShadowCoord;
    vec4 spotLightShadowCoord;
    //vec4 flashLightShadowCoord;
    mat3 TBN;
    //mat4 view;
    vec3 lightDirTangent;
    vec3 lightPositionTangent;
    vec3 viewDirTangent;
} vs_out;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;          // tangent space
    //sampler2D height1;
    //sampler2D emission;         // Texture used used for emission on the object itself (no emissionMap involved, just 100% coverage)
    //float emissionStrength;     // Strength for emission
    float shininess;            // Impacts the scattering/radius of the specular highlight
    //sampler2D flashLightEmissionMap;    // EmissionMap used for flashLight emission
    //sampler2D flashLightEmissionTexture; // Texture used for flashLight emission
};   
uniform Material material;

struct DirLight {
    vec3 direction;     // View Space
    vec3 color;         // Light color
    float ambient;      // Ambient strength
    float strength;     // Overall strength
    sampler2D depthMap;
};
uniform DirLight dirLight;

//struct PointLight {
//    vec3 position;      // View Space
//    vec3 color;         // Light color
//    float constant;     // Usually kept at 1.0f
//    float linear;       // Short distance intensity
//    float quadratic;    // Long distance intensity
//    float strength;     // Overall strength
//};
//uniform int pointLightsCount;       // For the loop
//uniform PointLight pointLights[4];  // Hard limit pointlights count (max 166?)
//
struct SpotLight {
    vec3 position;      // View Space
    vec3 direction;     // View Space
    float outerCutOff;  // Outer cone
    float epsilon;      // Gradually fade the light between inner and outer cone
    vec3 color;         // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
    sampler2D depthMap;
};
uniform SpotLight spotLight;
//
//struct FlashLight {
//    bool on;            // Flashlight on or off
//    float outerCutOff;  // Outer cone
//    float epsilon;      // Gradually fade the light between inner and outer cone
//    vec3 color;         // Light color
//    float constant;     // Usually kept at 1.0f
//    float linear;       // Short distance intensity
//    float quadratic;    // Long distance intensity
//    float strength;     // Overall strength
//    vec3 origin;        // 0.0f, 0.0f, 0.0f == shines straight from the center/camera
////    vec3 direction;
//    float emissionStrength;     // Overall strength
//    sampler2D depthMap;
//};
//uniform FlashLight flashLight;

//vec3 normalView = normalize(vs_out.NormalView);
vec3 normalTangent = normalize(texture(material.normal1, vs_out.TexCoords).rgb * 2.0 - 1.0); // tangent space

//vec3 viewDirView = vs_out.TBN * normalize(vec3(0,0,0) - vs_out.FragPosView); // tangent space

vec3 textureDiffuse = vec3(texture(material.diffuse1, vs_out.TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, vs_out.TexCoords));

vec3 CalcDirLight(DirLight light)
{
    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    float diff = max(dot(normalTangent, vs_out.lightDirTangent), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(vs_out.lightDirTangent + vs_out.viewDirTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);

    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);

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

    return (ambient + diffuse + specular) * light.strength;
}

//vec3 CalcPointLight(PointLight light)
//{
//    light.position = vs_out.TBN * normalize(light.position);
//    vec3 FragPosTangent = vs_out.TBN * vs_out.FragPosView; ///////////////
//
//    vec3 lightDir = normalize(light.position - vs_out.FragPosView);
//
//    // diffuse
//    float diff = max(dot(normalView, lightDir), 0.0f);
//    vec3 diffuse = light.color * diff * textureDiffuse;
//
//    // specular
//    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
//    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);
//    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
//    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.color * spec * textureSpecular;
//
//    // attenuation
//    float distance = length(light.position - vs_out.FragPosView);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    //float attenuation = 1.0f / (distance * distance);
//    diffuse *= attenuation;
//    specular *= attenuation;
//
//    return (diffuse + specular) * light.strength;
//}

vec3 CalcSpotLight(SpotLight light)
{   
    //light.position = vs_out.TBN * light.position;
    //light.direction = vs_out.TBN * light.direction;

    //vs_out.lightPositionTangent

    vec3 lightDir = normalize(vs_out.lightPositionTangent - vs_out.FragPosTangent);

    // diffuse
    float diff = max(dot(normalTangent, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // specular
    vec3 halfwayDir = normalize(lightDir + vs_out.viewDirTangent); // Blinn-Phong
    float spec = pow(max(dot(normalTangent, halfwayDir), 0.0f), material.shininess);
    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.color * spec * textureSpecular;

    // cone
    float theta = dot(lightDir, normalize(-vs_out.lightDirTangent));
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(vs_out.lightPositionTangent - vs_out.FragPosTangent);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    //float attenuation = 1.0f / (distance * distance);
    diffuse  *= attenuation;
    specular *= attenuation;

    // shadow - 1.0f = no shadow, 0.0f = full shadow
    float shadow = 0.0f;
    // perform perspective divide
    vec3 ShadowCoord = vs_out.spotLightShadowCoord.xyz / vs_out.spotLightShadowCoord.w;
    // convert from normalized device coordinates (in range [-1, 1]) to texture coordinates (in range [0, 1])
    ShadowCoord = ShadowCoord * 0.5f + 0.5f;
    // prevent shadow acne 
    //float bias = max(0.05 * (1.0 - dot(normalView, lightDir)), 0.005); // used twice dot(normalView, lightDir) // TODO sommige schaduwen verdwijnen?
    //float bias = 0.00005; // fixed

    // 1 sample
    //    if (texture(depthMap, ShadowCoord2.xy).x < ShadowCoord2.z) - bias ) {
    //        visibility = 0.25f;
    //    }
    
    // PCF 9 samples
    // ShadowCoord2.xy = coordinates on shadowMap
    // texture(shadowMap, ShadowCoord2.xy).x = closest depth from the light's point of view (or use .r instead of .x)
    // ShadowCoord2.z = distance from light from the current camera's point of view
    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
        }    
    }
    shadow /= 9.0f;
    // Outside the farPlane region of the light's frustum keep the shadow at 1.0f, not to cast shadows
    if(ShadowCoord.z > 1.0f)
        shadow = 1.0f;

    return (diffuse + specular) * light.strength;
}

//// Without Emission
////vec3 CalcFlashLight(FlashLight light)
////{   
////    vec3 lightDir = normalize(light.origin - vs_out.FragPosView);
////    // diffuse
////    float diff = max(dot(normalView, lightDir), 0.0f);
////    vec3 diffuse = light.color * diff * textureDiffuse;
////    // specular
////    vec3 reflectDir = reflect(-lightDir, normalView);
////    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
////    vec3 specular = light.color * spec * textureSpecular;
////    // cone
////    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
////    float theta = dot(lightDir, cameraDirection);
////    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / light.epsilon);
////    diffuse *= intensity;
////    specular *= intensity;
////    // attenuation
////    float distance = length(light.origin - vs_out.FragPosView);
////    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
////    diffuse  *= attenuation;
////    specular *= attenuation;
////
////    return (diffuse + specular) * light.strength;
////}

//vec3 CalcFlashLight(FlashLight light)
//{
//    light.origin = vs_out.TBN * light.origin;
//    
//    vec3 lightDir = normalize(light.origin - vs_out.FragPosView);
//
//    // diffuse
//    float diff = max(dot(normalView, lightDir), 0.0f);
//    vec3 diffuse = light.color * diff * textureDiffuse;
//
//    // specular
//    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
//    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);
//    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
//    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.color * spec * textureSpecular;
//
//    // cone
//    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis // deze moet geupdated worden, die zit al ergens in voor niet normal mapping... ?
//
//    //vec3 cameraDirection = light.direction;
//    float theta = dot(lightDir, cameraDirection);
//
//    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);
//    diffuse *= intensity;
//    specular *= intensity;
//
//    // attenuation
//    float distance = length(light.origin - vs_out.FragPosView);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    //float attenuation = 1.0f / distance; // linear
//    diffuse *= attenuation;
//    specular *= attenuation;   
//
//    // shadow - 1.0f = no shadow, 0.0f = full shadow
//    float shadow = 0.0f;
//    // perform perspective divide
//    vec3 ShadowCoord = vs_out.flashLightShadowCoord.xyz / vs_out.flashLightShadowCoord.w;
//    // convert from normalized device coordinates (in range [-1, 1]) to texture coordinates (in range [0, 1])
//    ShadowCoord = ShadowCoord * 0.5 + 0.5;
//    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
//    for(int x = -1; x <= 1; ++x) {
//        for(int y = -1; y <= 1; ++y) {
//            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
//        }    
//    }
//    shadow /= 9.0f;    
//    // Outside the farPlane region of the light's frustum keep the shadow at 1.0f, not to cast shadows
//    if(ShadowCoord.z > 1.0f) {
//        shadow = 1.0f;
//    }
//
//    // emission (put them in main if needed by other functions, slower)
//    vec3 textureFlashlightEmissionMap = vec3(texture(material.flashLightEmissionMap, vs_out.TexCoords));
//    vec3 textureFlashlightEmissionTexture = vec3(texture(material.flashLightEmissionTexture, vs_out.TexCoords));
//    // #1. emission: using specularMap as stamp, with if statement
//    //vec3 emission = vec3(0.0f);                         // Default no textureEmission visible
//    //if (textureSpecular.r == 0.0f) {                    // if textureSpecular == black (or whatever you choose)
//    //    emission = light.emission * textureEmission;    // show textureEmission
//    //}
//    // #2. emission: using specularMap as stamp, no if statement, a bit quicker, but less flexible (disable textureEmissionResult calculation)
//    //vec3 emission = textureSpecular.r * light.emission * textureEmissionResult;
//    // #3. emission: using specific emissionMap as stamp, a bit slower
//    vec3 emission = textureFlashlightEmissionMap.r * light.emissionStrength * textureFlashlightEmissionTexture;
//    emission *= shadow;
//    emission *= intensity;
//    emission *= attenuation;
//
//    return shadow * (diffuse + specular + emission) * light.strength;
//}
//
void main()
{
//    vec3 resultDirLight = CalcDirLight(dirLight) * 1.0f;
//
//    FragColor = vec4(resultDirLight, 1.0);

//
//    vec3 resultPointLight;
//    for(int i = 0; i < pointLightsCount; i++)
//        resultPointLight += CalcPointLight(pointLights[i]);
//
//
//
    vec3 resultSpotLight = CalcSpotLight(spotLight);
    FragColor = vec4(resultSpotLight, 1.0);
//
//    vec3 resultFlashLight = vec3(0.0f);
//    if (flashLight.on)
//        resultFlashLight = CalcFlashLight(flashLight);       
//
//
//
//
//
//    // emission - no emissionMap involved, just 100% coverage
//    vec3 emissionMaterial = texture(material.emission, vs_out.TexCoords).rgb * material.emissionStrength;
//
//    FragColor = vec4(resultDirLight + resultSpotLight + resultPointLight + resultFlashLight + emissionMaterial, 1.0);
}