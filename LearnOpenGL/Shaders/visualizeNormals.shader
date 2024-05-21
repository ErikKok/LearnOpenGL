#shader vertex
#version 420 core

// Visualize normals
// See https://learnopengl.com/Advanced-OpenGL/Geometry-Shader

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vs_out.normal = NormalMatrix * aNormal;
    gl_Position = modelViewMatrix * vec4(aPos, 1.0f);
}

#shader geometry
#version 420 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

layout (std140, binding = 0) uniform projectionUbo
{
    mat4 projection;   
};

const float MAGNITUDE = 0.2;

void main()
{
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = projection * gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = projection * (gl_in[i].gl_Position + vec4(gs_in[i].normal, 0.0f) * MAGNITUDE);
        EmitVertex();
        EndPrimitive();
    }
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}  