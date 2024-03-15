#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;   
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0); // combined
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0); // * mix(texture(texture0, TexCoord), texture(texture1, TexCoord2), mixAmount);
}


//Few things to look out for:
//- When doing lighting in view-space all relevant vectors need to be in view space. Since you're doing diffuse and specular this means light.direction should be in view space, but also the normal vector, and the view vector (doing a reflect on two view-space vectors will already give the result in view-space).
//- When multiplying world-space vectors with the view matrix you want to make sure whether you're dealing with a direction or a position vector. For a position vector you want its w component to be 1.0 so translation is included. For a direction vector you want the w component to be 0.0. The light, view and normal direction are all direction vectors so make sure to multiply them with a w component of 0.0: like

//vec3 lightDir = vec3(View * vec4(light.direction, 0.0))

//- Also make sure all vectors are unit vectors (normalized) when comparing/dot-producting them.