#shader vertex
#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Use an out vec3
out vec3 Color;

// Or you can use an Interface Block
//out VS_OUT {
//    vec3 ColorArray;
//} vs_out;

layout (std140, binding = 0) uniform uboProjectionView
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    Color = aColor; // vs_out.Color = ColorArray
}

#shader geometry
#version 420 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out;

in vec3 Color[];
out vec3 Color2;

//in VS_OUT {
//    vec3 ColorArray;
//} gs_in[];

//out vec3 ColorArray;

void main()
{  
    for (int i = 0; i < gl_in.length(); i++) {
        Color2 = Color[i];
        //ColorArray = gs_in[i].ColorArray;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}

#shader fragment
#version 410 core // 330 for ColorArray
out vec4 FragColor;

in vec3 Color2;

//in vec3 ColorArray;

void main()
{
    FragColor = vec4(Color2, 1.0f); // ColorArray
}