#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragPosView;
    vec3 NormalView;
    vec3 dirLightShadowCoord;               // clip space   // Orthographic
    vec3 dirLightDirectionView;
    vec3 pointLightPositionView[4];
    vec4 spotLightShadowCoord[2];           // clip space   // Perspective
    vec3 spotLightPositionView[2];
    vec3 spotLightDirectionView[2];
} vs_out;

layout(binding = 0, std430) readonly buffer dirLightMVPSSBO {
    mat4 dirLightMVP[]; };

layout(binding = 1, std430) readonly buffer spotLight0MVPSSBO {
    mat4 spotLight0MVP[]; };

layout(binding = 2, std430) readonly buffer spotLight1MVPSSBO {
    mat4 spotLight1MVP[]; };

layout(binding = 3, std430) readonly buffer normalMatrixSSBO {
    mat4 normalMatrix[]; };

layout(binding = 4, std430) readonly buffer modelViewMatrixSSBO {
    mat4 modelViewMatrix[]; };

layout(binding = 5, std430) readonly buffer MVPSSBO {
    mat4 MVP[]; };

uniform vec3 dirLightDirection;      // View Space // normalized
uniform vec3 pointLightPosition[4];  // View Space // NOT normalized
uniform vec3 spotLightPosition[2];   // View Space // NOT normalized
uniform vec3 spotLightDirection[2];  // View Space // NOT normalized

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.NormalView = mat3(normalMatrix[gl_InstanceID]) * aNormal;
    vs_out.FragPosView = vec3(modelViewMatrix[gl_InstanceID] * vec4(aPos, 1.0f));
    // dirLight
    vs_out.dirLightShadowCoord = vec3(dirLightMVP[gl_InstanceID] * vec4(aPos, 1.0f));
    vs_out.dirLightDirectionView = dirLightDirection;
    // pointLight
    for (int i = 0; i < vs_out.pointLightPositionView.length(); i++)
        vs_out.pointLightPositionView[i] = pointLightPosition[i];
    // spotLight
    vs_out.spotLightShadowCoord[0] = spotLight0MVP[gl_InstanceID] * vec4(aPos, 1.0f); // spotLight0MVP kan een loop/struct worden
    vs_out.spotLightShadowCoord[1] = spotLight1MVP[gl_InstanceID] * vec4(aPos, 1.0f);
    for (int i = 0; i < vs_out.spotLightDirectionView.length(); i++) {
        vs_out.spotLightDirectionView[i] = spotLightDirection[i];
        vs_out.spotLightPositionView[i] = spotLightPosition[i];
    }

    gl_Position = MVP[gl_InstanceID] * vec4(aPos, 1.0f); // clip space
}

//shader geometry
//#version 420 core
//layout (triangles) in;
//layout (triangle_strip, max_vertices = 3) out;
//
//in VS_OUT { // PASS_THROUGH_GS
//    vec2 TexCoords;
//    vec3 FragPosView;
//    vec3 NormalView;
//} gs_in[];
//
//out VS_OUT { // PASS_THROUGH_GS
//    vec2 TexCoords;
//    vec3 FragPosView;
//    vec3 NormalView;
//} gs_out;
//
//void main()
//{
//    for (int i = 0; i < gl_in.length(); i++) {
//        gs_out.TexCoords = gs_in[i].TexCoords;
//        gs_out.FragPosView = gs_in[i].FragPosView;
//        gs_out.NormalView = gs_in[i].NormalView;
//        gl_Position = gl_in[i].gl_Position;
//        EmitVertex();
//        EndPrimitive();
//    }
//}

#shader fragment
#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPosView;
    vec3 NormalView;
    vec3 dirLightShadowCoord;
    vec3 dirLightDirectionView;
    vec3 pointLightPositionView[4];
    vec4 spotLightShadowCoord[2];
    vec3 spotLightPositionView[2];
    vec3 spotLightDirectionView[2];
} vs_out;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
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

vec3 normalViewNormalized = normalize(vs_out.NormalView);
vec3 FragPosViewNormalized = normalize(vs_out.FragPosView);

vec3 textureDiffuse = vec3(texture(material.diffuse1, vs_out.TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, vs_out.TexCoords));

float cutOffSpecular = 0.1f;
float cutOffAttenuation = 0.01f;

vec3 CalcDirLight(DirLight light)
{
    // ambient
    vec3 ambient = light.ambient * textureDiffuse;

    // diffuse
    float diff = max(dot(normalViewNormalized, vs_out.dirLightDirectionView), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;

    // Outside depthMap/frustum?
    if (vs_out.dirLightShadowCoord.x > 1.0f || vs_out.dirLightShadowCoord.x < -1.0f 
     || vs_out.dirLightShadowCoord.y > 1.0f || vs_out.dirLightShadowCoord.y < -1.0f 
     || vs_out.dirLightShadowCoord.z > 1.0f || vs_out.dirLightShadowCoord.z < -1.0f)
        return (ambient + diffuse) * light.strength;

    // shadow
    vec3 ShadowCoord = vs_out.dirLightShadowCoord.xyz * 0.5f + 0.5f;
    float shadow = texture(light.depthMap, ShadowCoord.xyz);

    // no specular lighting in last part of the frustum
    if (ShadowCoord.z > 0.75f)
        return shadow * (ambient + diffuse) * light.strength;

    // specular
    vec3 halfwayDir = normalize(vs_out.dirLightDirectionView - FragPosViewNormalized); // Blinn-Phong
    float spec = pow(max(dot(normalViewNormalized, halfwayDir), 0.0f), material.shininess);

    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);

    vec3 specular = light.color * spec * textureSpecular;

    return shadow * (ambient + diffuse + specular) * light.strength;
}

vec3 CalcPointLight(PointLight light, int i)
{
    // attenuation
    float distance = length(vs_out.pointLightPositionView[i] - vs_out.FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //float attenuation = 1.0f / (distance * distance);
    // smooth attentuation to zero if x amount of light remaining (attenuation will never reach 0.0f)
    attenuation = (attenuation - cutOffAttenuation) / (1.0f - cutOffAttenuation);
    attenuation = max(attenuation, 0.0f);
    
    // Earyl return here is a bit slower
    // if (attenuation == 0.0f)
    //    return vec3(0.0f);

    vec3 lightDir = normalize(vs_out.pointLightPositionView[i] - vs_out.FragPosView);

    // diffuse
    float diff = max(dot(normalViewNormalized, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;
    diffuse *= attenuation;

    // specular (only calculate specular above a certain amount of light remaining)
    vec3 specular;
    if (attenuation > cutOffSpecular) {
        vec3 halfwayDir = normalize(lightDir - FragPosViewNormalized); // Blinn-Phong
        float spec = pow(max(dot(normalViewNormalized, halfwayDir), 0.0f), material.shininess);
        //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
        //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
        specular = light.color * spec * textureSpecular;
        specular *= attenuation;
    }

    return (diffuse + specular) * light.strength;
}

vec3 CalcSpotLight(SpotLight light, int i)
{     
    // perform perspective divide
    vec3 ShadowCoord = vs_out.spotLightShadowCoord[i].xyz / vs_out.spotLightShadowCoord[i].w;
    // Outside depthMap/frustum?
    if (ShadowCoord.x > 1.0f || ShadowCoord.x < -1.0f || ShadowCoord.y > 1.0f  || ShadowCoord.y < -1.0f || ShadowCoord.z > 1.0f || ShadowCoord.z < 0.0f)
        return vec3(0.0f);

    // convert from normalized device coordinates (in range [-1, 1]) to texture coordinates (in range [0, 1])
    ShadowCoord = ShadowCoord * 0.5f + 0.5f;

    // sampler2DShadow
    float shadow = texture(light.depthMap, ShadowCoord.xyz);

    // attenuation
    float distance = length(vs_out.spotLightPositionView[i] - vs_out.FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    //float attenuation = 1.0f / (distance * distance);
    attenuation = (attenuation - cutOffAttenuation) / (1.0f - cutOffAttenuation);
    attenuation = max(attenuation, 0.0f);

    vec3 lightDir = normalize(vs_out.spotLightPositionView[i] - vs_out.FragPosView);

    // cone
    float theta = dot(lightDir, normalize(-vs_out.spotLightDirectionView[i]));
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / light.epsilon);

    // diffuse
    float diff = max(dot(normalViewNormalized, lightDir), 0.0f);
    vec3 diffuse = light.color * diff * textureDiffuse;
    diffuse *= attenuation;
    diffuse *= intensity;

    // specular
    vec3 specular;
    if (attenuation > cutOffSpecular) {
        vec3 halfwayDir = normalize(lightDir - FragPosViewNormalized); // Blinn-Phong
        float spec = pow(max(dot(normalViewNormalized, halfwayDir), 0.0f), material.shininess);
        //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
        //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
        specular = light.color * spec * textureSpecular;
        specular *= attenuation;
        specular *= intensity;
    }
    //float shadow = 0.0f;

    //vec3 ShadowCoord = vs_out.spotLightShadowCoord[i].xyz / vs_out.spotLightShadowCoord[i].w;
    // convert from normalized device coordinates (in range [-1, 1]) to texture coordinates (in range [0, 1])
    //ShadowCoord = ShadowCoord * 0.5f + 0.5f;
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
    // vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    // for(int x = -1; x <= 1; ++x) {
    //     for(int y = -1; y <= 1; ++y) {
    //         shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
    //     }    
    // }
    // shadow /= 9.0f;
    // float shadow = texture(light.depthMap, ShadowCoord.xyz);// + vec2(x, y) * texelSize);
    // // Outside the farPlane region of the light's frustum keep the shadow at 1.0f, not to cast shadows
    // if(ShadowCoord.z > 1.0f)
    //     shadow = 1.0f;
    //
    // float shadow = 1.0f;
    // Only calculate shadows inside the lights frustum, outside the farPlane region (.z > 1.0f) keep the shadow at 1.0f, not to cast shadows
    // if (ShadowCoord.z <= 1.0f) 
    //     shadow = texture(light.depthMap, ShadowCoord.xyz);

    vec3 emission;
    if (light.emissionStrength > 0.0f) {
        // TODO optimization: these 2 calculations are the same for every spotLight, but are only needed if ONE of them has emissionStrength > 0.0f
        vec3 textureLightEmissionMap = vec3(texture(material.lightEmissionMap, vs_out.TexCoords));
        vec3 textureLightEmissionTexture = vec3(texture(material.lightEmissionTexture, vs_out.TexCoords));
        // #1. emission: using specularMap as stamp, with if statement
        //vec3 emission = vec3(0.0f);                         // Default no textureEmission visible
        //if (textureSpecular.r == 0.0f) {                    // if textureSpecular == black (or whatever you choose)
        //    emission = light.emission * textureEmission;    // show textureEmission
        //}
        // #2. emission: using specularMap as stamp, no if statement, a bit quicker, but less flexible (disable textureEmissionResult calculation)
        //vec3 emission = textureSpecular.r * light.emission * textureEmissionResult;
        // #3. emission: using specific emissionMap as stamp, a bit slower
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