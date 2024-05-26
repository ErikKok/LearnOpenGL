#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos; // Normalized device coordinates of a cube ranging from -1 to 1

uniform mat4 inverseMatrix; // Inverse depthMapDirLightFBO ViewProjectionMatrix // World space
uniform mat4 viewProjectionMatrix; // VPM of the current scene in wich to render the frustum

void main()
{
    vec4 aPosWorld = inverseMatrix * vec4(aPos, 1.0f);
    aPosWorld = aPosWorld / aPosWorld.w; // perform perspective divide for orthographic frustum
    gl_Position = viewProjectionMatrix * aPosWorld;
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
}

//Resources:
//https://community.khronos.org/t/draw-frustum-outlines/49440/3
//https://webglfundamentals.org/webgl/lessons/webgl-visualizing-the-camera.html
//https://community.khronos.org/t/draw-frustrum-in-opengl/71997/6
//https://stackoverflow.com/questions/24085238/how-to-draw-frustum-in-opengl?rq=3