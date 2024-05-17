#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;         // local space
layout (location = 1) in vec2 aTexCoords;   // range [0,1] 
layout (location = 2) in vec3 aNormal;

out VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosView; // view space
    vec3 NormalView;  // view space
    vec4 ShadowCoordOrtho; // clip space
    vec4 ShadowCoordPerspective; // clip space ??????????
} vs_out;

layout(binding = 2, std430) readonly buffer NormalMatrixSSBO {
    mat4 NormalMatrix[];
};

layout(binding = 3, std430) readonly buffer ModelViewMatrixSSBO {
    mat4 modelViewMatrix[];
};

layout(binding = 4, std430) readonly buffer MVPMatrixSSBO {
    mat4 MVPMatrix[];
};

layout(binding = 5, std430) readonly buffer LightOrthoMVPMatrixSSBO {
    mat4 LightOrthoMVPMatrix[];
};

layout(binding = 6, std430) readonly buffer LightPerspectiveMVPMatrixSSBO {
    mat4 LightPerspectiveMVPMatrix[];
};

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosView = vec3(modelViewMatrix[gl_InstanceID] * vec4(aPos, 1.0));
    vs_out.NormalView = mat3(NormalMatrix[gl_InstanceID]) * aNormal;
    vs_out.ShadowCoordOrtho = LightOrthoMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0);             // ortho
    vs_out.ShadowCoordPerspective = LightPerspectiveMVPMatrix[gl_InstanceID] * vec4(aPos, 1.0); // perspective
    gl_Position = MVPMatrix[gl_InstanceID] * vec4(aPos, 1.0); // clip space
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

in VS_OUT { // PASS_THROUGH_GS
    vec2 TexCoords;
    vec3 FragPosView;
    vec3 NormalView;
    vec4 ShadowCoordOrtho;
    vec4 ShadowCoordPerspective;
} vs_out;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    //sampler2D normal1;
    //sampler2D height1;
    sampler2D emission;
    float emissionStrength;
    sampler2D flashlightMap;
    sampler2D flashlightResult;
    float shininess;    // Impacts the scattering/radius of the specular highlight
};   
uniform Material material;

struct DirLight {
    vec3 direction;     // View Space
    vec3 diffuse;       // Light color
    float ambient;      // Ambient strength
    float strength;     // Overall strength
    sampler2D depthMap;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;      // View Space
    vec3 diffuse;       // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
};
uniform int pointLightsCount;       // For the loop
uniform PointLight pointLights[4];  // Hard limit pointlights count (max 166?)

struct SpotLight {
    vec3 position;      // View Space
    vec3 direction;     // View Space
    float outerCutOff;  // Outer cone
    float epsilon;      // Gradually fade the light between inner and outer cone
    vec3 diffuse;       // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
    sampler2D depthMap;
};
uniform SpotLight spotLight;

struct FlashLight {
    bool on;            // Flashlight on or off
    float outerCutOff;  // Outer cone
    float epsilon;      // Gradually fade the light between inner and outer cone
    vec3 diffuse;       // Light color
    float constant;     // Usually kept at 1.0f
    float linear;       // Short distance intensity
    float quadratic;    // Long distance intensity
    float strength;     // Overall strength
    vec3 origin;        // 0.0f, 0.0f, 0.0f == shines straight from the center/camera
    float emissionStrength;     // Overall strength
};
uniform FlashLight flashLight;

vec3 normalView = normalize(vs_out.NormalView);
vec3 viewDirView = normalize(-vs_out.FragPosView);

vec3 textureDiffuse = vec3(texture(material.diffuse1, vs_out.TexCoords));
vec3 textureSpecular = vec3(texture(material.specular1, vs_out.TexCoords));

vec3 CalcDirLight(DirLight light)
{
    vec3 lightDir = normalize(light.direction);
    // ambient
    vec3 ambient = light.ambient * textureDiffuse;
    // diffuse
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);

    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);

    vec3 specular = light.diffuse * spec * textureSpecular;

    // shadow

    // convert from normalized device coordinates (in range [-1, 1]) to texture coordinates (in range [0, 1])
    // TODO more efficient to do this before sending to Shader?
    vec4 ShadowCoord = vs_out.ShadowCoordOrtho * 0.5 + 0.5;

    // perform perspective divide, nog nodig?
    // convert from clip space to normalized device coordinates:
    //ShadowCoord2 = vec4(ShadowCoord2.xyz / ShadowCoord2.w, 1.0f); // only needed for perspective projection, not orthographic projection
    //ShadowCoord2 = ShadowCoord2 / ShadowCoord2.w; // zelfde resultaat / berekening
    float shadow = 1.0f; // 1.0f = no shadow
    // ShadowCoord2.xy = coordinates on shadowMap
    // texture(shadowMap, ShadowCoord2.xy).x = closest depth from the light's point of view (or use .r instead of .x)
    // ShadowCoord2.z = distance from light from the current camera's point of view

    // prevent shadow acne 
    //float bias = max(0.05 * (1.0 - dot(normalView, lightDir)), 0.005); // used twice dot(normalView, lightDir) // sommige schaduwen verdwijnen?
    //float bias = 0.00005;
    //    if (texture(depthMap, ShadowCoord2.xy).x < ShadowCoord2.z) { // - bias ) {
    //        visibility = 0.25f;
    //    }
    
    // PCF 9 samples
    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            //float pcfDepth = texture(depthMap, ShadowCoord2.xy + vec2(x, y) * texelSize).r; 
            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
        }    
    }
    shadow /= 9.0f;

    // keep the shadow at 1.0, not to cast shadows outside the farPlane region of the light's frustum
    if(ShadowCoord.z > 1.0f)
        shadow = 1.0f;

    return shadow * (ambient + diffuse + specular) * light.strength;
    //return (ambient + diffuse + specular) * light.strength;
}

vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position - vs_out.FragPosView);
    // diffuse
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);
    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // attenuation
    float distance = length(light.position - vs_out.FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //float attenuation = 1.0f / (distance * distance);
    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.strength;
}

vec3 CalcSpotLight(SpotLight light)
{   
    vec3 lightDir = normalize(light.position - vs_out.FragPosView);
    // diffuse
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);
    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // cone
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / light.epsilon);
    diffuse *= intensity;
    specular *= intensity;
    // attenuation
    float distance = length(light.position - vs_out.FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));      
    //float attenuation = 1.0f / (distance * distance);
    diffuse  *= attenuation;
    specular *= attenuation;

    // shadow


    // perform perspective divide
    //ShadowCoord = ShadowCoord / ShadowCoord.w;
    vec3 ShadowCoord = vs_out.ShadowCoordPerspective.xyz / vs_out.ShadowCoordPerspective.w;

    ShadowCoord = ShadowCoord * 0.5 + 0.5; // dit was eerst vec4 of toch 3?

    float shadow = 1.0f; // 1.0f = no shadow
    // ShadowCoord2.xy = coordinates on shadowMap
    // texture(shadowMap, ShadowCoord2.xy).x = closest depth from the light's point of view (or use .r instead of .x)
    // ShadowCoord2.z = distance from light from the current camera's point of view

    // prevent shadow acne 
    //float bias = max(0.05 * (1.0 - dot(normalView, lightDir)), 0.005); // used twice dot(normalView, lightDir) // sommige schaduwen verdwijnen?
    //float bias = 0.00005;
    //    if (texture(depthMap, ShadowCoord2.xy).x < ShadowCoord2.z) { // - bias ) {
    //        visibility = 0.25f;
    //    }
    
    // PCF 9 samples
    vec2 texelSize = 1.0f / textureSize(light.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            //float pcfDepth = texture(depthMap, ShadowCoord2.xy + vec2(x, y) * texelSize).r; 
            shadow += ShadowCoord.z > (texture(light.depthMap, ShadowCoord.xy + vec2(x, y) * texelSize).x) ? 0.0f : 1.0f; // - bias removed
        }    
    }
    shadow /= 9.0f;

    // keep the shadow at 1.0, not to cast shadows outside the farPlane region of the light's frustum
    if(ShadowCoord.z > 1.0f)
        shadow = 1.0f;

    return shadow * (diffuse + specular) * light.strength;
}

// Without Emission
//vec3 CalcFlashLight(FlashLight light)
//{   
//    vec3 lightDir = normalize(light.origin - vs_out.FragPosView);
//    // diffuse
//    float diff = max(dot(normalView, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * textureDiffuse;
//    // specular
//    vec3 reflectDir = reflect(-lightDir, normalView);
//    float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
//    vec3 specular = light.diffuse * spec * textureSpecular;
//    // cone
//    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
//    float theta = dot(lightDir, cameraDirection);
//    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / light.epsilon);
//    diffuse *= intensity;
//    specular *= intensity;
//    // attenuation
//    float distance = length(light.origin - vs_out.FragPosView);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    diffuse  *= attenuation;
//    specular *= attenuation;
//
//    return (diffuse + specular) * light.strength;
//}

vec3 CalcFlashLight(FlashLight light)
{
    vec3 lightDir = normalize(light.origin - vs_out.FragPosView);
    // diffuse
    float diff = max(dot(normalView, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * textureDiffuse;
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDirView); // Blinn-Phong
    float spec = pow(max(dot(normalView, halfwayDir), 0.0f), material.shininess);
    //vec3 reflectDir = reflect(-lightDir, normalView); // Phong
    //float spec = pow(max(dot(viewDirView, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.diffuse * spec * textureSpecular;
    // emission calculations (put them in main if needed by other functions, slower)
    vec3 textureflashlightMap = vec3(texture(material.flashlightMap, vs_out.TexCoords));
    vec3 textureflashlightResult = vec3(texture(material.flashlightResult, vs_out.TexCoords));
    // #1. emission: using specularMap as stamp, with if statement
    //vec3 emission = vec3(0.0f);                         // Default no textureEmission visible
    //if (textureSpecular.r == 0.0f) {                    // if textureSpecular == black (or whatever you choose)
    //    emission = light.emission * textureEmission;    // show textureEmission
    //}
    // #2. emission: using specularMap as stamp, no if statement, a bit quicker, but less flexible (disable textureEmissionResult calculation)
    //vec3 emission = textureSpecular.r * light.emission * textureEmissionResult;
    // #3. emission: using specific emissionMap as stamp, a bit slower
    vec3 emission = textureflashlightMap.r * light.emissionStrength * textureflashlightResult;
    // cone
    vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f); // camera.m_front with negated z-axis
    float theta = dot(lightDir, cameraDirection);
    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / light.epsilon);
    diffuse *= intensity;
    specular *= intensity;
    emission *= intensity;
    // attenuation
    float distance = length(light.origin - vs_out.FragPosView);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //float attenuation = 1.0f / distance; // linear
    diffuse *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (diffuse + specular + emission) * light.strength;
}

void main()
{
    vec3 resultDirLight = CalcDirLight(dirLight);

    vec3 resultPointLight;
    for(int i = 0; i < pointLightsCount; i++)
        resultPointLight += CalcPointLight(pointLights[i]);

    vec3 resultFlashLight = vec3(0.0f);
    vec3 resultSpotLight = CalcSpotLight(spotLight);

    if (flashLight.on)
        resultFlashLight = CalcFlashLight(flashLight);       

    // emission
    vec3 emissionMaterial = texture(material.emission, vs_out.TexCoords).rgb * material.emissionStrength;

    FragColor = vec4(resultDirLight + resultSpotLight + resultPointLight + resultFlashLight + emissionMaterial, 1.0);
}