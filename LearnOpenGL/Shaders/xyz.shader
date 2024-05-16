#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Option 1: use an out vec3
out vec3 Color;

// Option 2: or you can use an Interface Block
//out VS_OUT {
//    vec3 ColorArray;
//} vs_out;

uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * vec4(aPos, 1.0);
    Color = aColor; // vs_out.Color = ColorArray
}

#shader geometry
#version 330 core
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
#version 330 core
out vec4 FragColor;

in vec3 Color2;

//in vec3 ColorArray;

void main()
{
    FragColor = vec4(Color2, 1.0f); // ColorArray
}