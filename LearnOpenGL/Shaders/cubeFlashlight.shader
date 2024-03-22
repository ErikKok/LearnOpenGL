#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal; // <------- enige verschil met cubeShader (1 ipv 2) + geen textures

out vec2 TexCoords;
out vec3 FragPos; // View space
out vec3 Normal;
out vec3 LightPos;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};
uniform vec3 lightPos;
uniform mat4 model;

void main()
{
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    TexCoords = aTexCoords;
    LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos; // positie fragment (Vertex) in view space
in vec3 Normal;
in vec3 LightPos; // positie lightcube in view space

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};   
uniform Material material;

struct Light {
    //vec3 position; // camera.getPosition() // in view space positon(0,0,0)
    //vec3 direction; // camera.getFront() // in view space positon(0,0,-1)
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
vec3 cameraDirection = vec3(0.0f, 0.0f, 1.0f);

void main()
{
    // Flashlight
    vec3 lightDir = normalize(cameraPosition - FragPos); // punt waaruit het licht komt, of naar toe gaat?
    float theta = dot(lightDir, normalize(cameraDirection));

    // Spotlight
    //vec3 lightDir = normalize(LightPos - FragPos); // punt waaruit het licht komt, of naar toe gaat?  
    //float theta = dot(lightDir, normalize(vec3(0.0f, 0.0f, 1.0f)));

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  	if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    { 
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

        // specular
        vec3 reflectDir = reflect(lightDir, norm);
        vec3 viewDir = normalize(-FragPos); // from what direction the player is looking at the fragment
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

        float distance    = length(LightPos - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    
        ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation;

        FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }
 }