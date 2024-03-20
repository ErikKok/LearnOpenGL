#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal; // <------- enige verschil met cubeShader (1 ipv 2) + geen textures

out vec2 TexCoords;
//out vec3 FragPosView;
out vec3 FragPosWorld;
//out vec3 NormalView;
out vec3 NormalWorld;
//out vec3 LightPosView;
out vec3 LightPosWorld;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};
uniform vec3 lightPos; // world space
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    //FragPosView = vec3(view * model * vec4(aPos, 1.0));
    FragPosWorld = vec3(model * vec4(aPos, 1.0));
    //NormalView = mat3(transpose(inverse(view * model))) * aNormal;
    NormalWorld = mat3(transpose(inverse(model))) * aNormal;
    //LightPosView = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    LightPosWorld = lightPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
//in vec3 FragPosView; // positie fragment (Vertex) in view space
in vec3 FragPosWorld;
//in vec3 NormalView;
in vec3 NormalWorld;
//in vec3 LightPosView;
in vec3 LightPosWorld; // positie lightcube in view space, maar niet normalized!?
in vec3 FragPosLightCube; // positie fragment (Vertex) in view space vanaf lightpos gezien

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};   
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
//vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f);

void main()
{
    vec3 lightDir = normalize(light.position - FragPosWorld);  
    
    float theta = dot(lightDir, normalize(-light.direction));

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  	if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    { 
        // diffuse 
        vec3 norm = normalize(NormalWorld);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

        // specular
        vec3 viewDir = normalize(cameraPosition - FragPosWorld); // from what direction the player is looking at the fragment
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        // emission
        vec3 emission = vec3(0.0);
        if (texture(material.specular, TexCoords).r == 0.0) {
        emission = vec3(texture(material.emission, TexCoords)) * 0.25f;
        }

        // spot Light

        //float epsilon = light.cutOff - light.outerCutOff;
        //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 0.7);
        //ambient *= intensity;
        //diffuse *= intensity;
        //specular *= intensity;

        float distance    = length(LightPosWorld - FragPosWorld);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    
        //ambient  *= attenuation; // if we were to use more than 1 light source all the ambient components will start to stack up
        diffuse  *= attenuation;
        specular *= attenuation;
        emission *= attenuation;

        FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }
 }